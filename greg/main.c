#include "arena.h"
#include "game_anim.h"
#include "game_player_control.h"
#include "grey_assert.h"
#include "grey_ecs.h"
#include "grey_events.h"
#include "grey_input.h"
#include "grey_tilemap.h"
#include "raylib.h"
#include "systems/grey_systems.h"

#include <stdint.h>
#include <string.h>
#include <threads.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define TARGET_FPS 60
#define ARENA_CAPACITY_MB (1 * 1024 * 1024)

#define TILE_SIZE 64
#define MAP_COLS 10
#define MAP_ROWS 10

#define PLAYER_START_X (SCREEN_WIDTH / 2.0f)
#define PLAYER_START_Y (SCREEN_HEIGHT / 2.0f)
#define PLAYER_SPRITE_SIZE 64
#define PLAYER_FRAME_WIDTH 16
#define PLAYER_FRAME_HEIGHT 32

#define CAM_DEADZONE_X 200.0f
#define CAM_DEADZONE_Y 150.0f
#define CAM_SPEED 5.0f

u64 grey_game_asset_path_len = 0;
char *grey_game_asset_path = nullptr;
u64 grey_asset_path_len = 0;
char *grey_asset_path;
Arena fpath_arena;

bool asset_path_init() {
#ifndef GREY_TARGETS_ANDROID
  GREY_ASSERT(!grey_game_asset_path, "Asset path already initialized.");

  fpath_arena = arena_create(4 * 1024);
  if (!fpath_arena)
    return false;

  const char *raylib_app_dir = GetApplicationDirectory();
  GREY_ASSERT(raylib_app_dir, "Raylib failed to get application directory.");

  char *assets = "/assets/";
  u64 strlen_raylib_app_dir = strlen(raylib_app_dir);
  u64 strlen_assets = strlen(assets);
  grey_game_asset_path_len = strlen_raylib_app_dir + strlen_assets;
  char *path = arena_allocate(fpath_arena, grey_game_asset_path_len + 1);
  memcpy(path, raylib_app_dir, strlen_raylib_app_dir);
  memcpy(path + strlen_raylib_app_dir, assets, strlen_assets);
  path[grey_game_asset_path_len] = 0;
  grey_game_asset_path = path;
#endif
  return true;
}

void asset_path_destroy() {
#ifndef GREY_TARGETS_ANDROID
  GREY_ASSERT(grey_game_asset_path, "Asset path is null.");
  arena_destroy(fpath_arena);
  grey_game_asset_path = nullptr;
  grey_game_asset_path_len = 0;
#endif
}

char *asset_path(const char *asset) {
  // user should just copy the path
#ifdef GREY_TARGETS_ANDROID
  return (char *)asset;
#else
  GREY_ASSERT(fpath_arena,
              "Asset path arena is null. Did you call asset_path_init()?");

  u64 strlen_asset = strlen(asset);
  u64 path_len = grey_game_asset_path_len + strlen_asset;

  if (grey_asset_path_len < path_len) {
    grey_asset_path_len = path_len;
    grey_asset_path = arena_allocate(fpath_arena, path_len + 1);
  }

  memcpy(grey_asset_path, grey_game_asset_path, grey_game_asset_path_len);
  memcpy(grey_asset_path + grey_game_asset_path_len, asset, strlen_asset);

  grey_asset_path[path_len] = 0;
  return grey_asset_path;
#endif
}

// Define a small 10x10 map. (1 = Grass, 2 = Wall, 3 = Water)
u8 level_one_data[MAP_ROWS][MAP_COLS] = {
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, {2, 1, 1, 1, 1, 1, 1, 1, 3, 2},
    {2, 1, 1, 1, 1, 1, 1, 3, 3, 2}, {2, 1, 1, 1, 1, 1, 1, 3, 3, 2},
    {2, 2, 2, 1, 1, 1, 1, 1, 1, 2}, {2, 1, 1, 1, 1, 1, 1, 1, 1, 2},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 2}, {2, 1, 1, 1, 1, 1, 1, 1, 1, 2},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 2}, {2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
};

int main(void) {

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Grey Engine: M3");
  ToggleFullscreen();
  SetTargetFPS(TARGET_FPS);

  grey_input_init(GREY_JOYSTICK);
  grey_default_action_btn();
  asset_path_init();

  Arena arena = arena_create(ARENA_CAPACITY_MB);

  EcsRegistry reg = ecs_create(arena);

  GreyEvents events_system = event_system_create(arena);

  Texture2D player_sprite = LoadTexture(asset_path("character.png"));
  Entity player = ecs_create_entity(reg);

  ecs_add_position(reg, player, PLAYER_START_X, PLAYER_START_Y);
  ecs_add_render(reg, player, PLAYER_SPRITE_SIZE, PLAYER_SPRITE_SIZE, BLUE);
  ecs_add_player(reg, player);
  ecs_add_sprite(reg, player, player_sprite,
                 (Rectangle){0, 0, PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT},
                 (SpriteSize){PLAYER_SPRITE_SIZE, PLAYER_SPRITE_SIZE}, WHITE);
  ecs_add_velocity(reg, player);

  AnimClip anim[] = {
      [IDLE_DOWN] = {.frame_time = 1,
                     .max_frame = 1,
                     .start_x = 0,
                     .start_y = 0},
      [IDLE_RIGHT] = {.frame_time = 1,
                      .max_frame = 1,
                      .start_x = 0,
                      .start_y = 32},
      [IDLE_UP] = {.frame_time = 1,
                   .max_frame = 1,
                   .start_x = 0,
                   .start_y = 64},
      [IDLE_LEFT] = {.frame_time = 1,
                     .max_frame = 1,
                     .start_x = 0,
                     .start_y = 96},
      [WALK_DOWN] = {.frame_time = 0.10f,
                     .max_frame = 4,
                     .start_x = 0,
                     .start_y = 0},
      [WALK_RIGHT] = {.frame_time = 0.10f,
                      .max_frame = 4,
                      .start_x = 0,
                      .start_y = 32},
      [WALK_UP] = {.frame_time = 0.10f,
                   .max_frame = 4,
                   .start_x = 0,
                   .start_y = 64},
      [WALK_LEFT] = {.frame_time = 0.10f,
                     .max_frame = 4,
                     .start_x = 0,
                     .start_y = 96},
  };

  ecs_add_animator(reg, player, anim, IDLE_DOWN);
  ecs_add_collider(reg, player, 24, 8, 20, 52, false, 0);

  auto camera_entity = ecs_create_entity(reg);
  ecs_add_camera(
      reg, camera_entity, (Vector2){CAM_DEADZONE_X, CAM_DEADZONE_Y},
      (Vector2){PLAYER_START_X, PLAYER_START_Y}, player, CAM_SPEED,
      (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
      (Vector2){740 - (SCREEN_WIDTH / 2), 640 - (SCREEN_HEIGHT / 2)});

  Camera2D camera = {
      .target = {PLAYER_START_X, PLAYER_START_Y},
      .offset = {(SCREEN_WIDTH / 2.0f) - (PLAYER_SPRITE_SIZE / 2.0f),
                 (SCREEN_HEIGHT / 2.0f) - (PLAYER_SPRITE_SIZE / 2.0f)},
      .rotation = 0.0f,
      .zoom = 1.0f};

  u8 *one_way_trigger[MAX_ENTITIES] = {nullptr};

  Entity one_way_entry = ecs_create_entity(reg);
  ecs_add_position(reg, one_way_entry, 2 * TILE_SIZE, 4 * TILE_SIZE - 4);
  ecs_add_collider(reg, one_way_entry, TILE_SIZE, 4, 2, 0, true,
                   ONE_WAY_ENTRY_TRIGGER);

  Entity one_way_exit = ecs_create_entity(reg);
  ecs_add_position(reg, one_way_exit, 2 * TILE_SIZE, 5 * TILE_SIZE);
  ecs_add_collider(reg, one_way_exit, 60, 4, 2, 8, true, ONE_WAY_EXIT_TRIGGER);

  Entity one_way_block = ecs_create_entity(reg);
  ecs_add_position(reg, one_way_block, 3 * TILE_SIZE, 4 * TILE_SIZE);
  ecs_add_collider(reg, one_way_block, 4, TILE_SIZE, 0, 0, false, 0);

  one_way_trigger[one_way_entry] = &level_one_data[4][2];
  one_way_trigger[one_way_exit] = &level_one_data[4][2];

  TileDef tile_set[] = {
      [1] = {.solid = false, .trigger = false},
      [2] = {.solid = true, .trigger = false},
      [3] = {.solid = false, .trigger = true, .trigger_id = WATER_TILE}};

  GreyTileMap map = {(u8 *)level_one_data, MAP_ROWS, MAP_COLS, TILE_SIZE};

  while (!WindowShouldClose()) {
    event_system_reset(events_system);

    grey_input_begin_frame();
    grey_input_update();

    game_player_control(reg);

    grey_sys_physics_update(reg, &map, tile_set, events_system);
    grey_sys_animator_update(reg);

    game_system_trigger_update(events_system, reg, camera_entity,
                               one_way_trigger);

    grey_sys_camera_update(reg, camera_entity, &camera);

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode2D(camera);

    grey_draw_tilemap(&map);
    grey_sys_render_draw(reg);

    EndMode2D();

    grey_input_draw_gamepad();

    EndDrawing();
  }

  UnloadTexture(player_sprite);
  asset_path_destroy();
  CloseWindow();

  return 0;
}
