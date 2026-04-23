#include "arena.h"
#include "game_anim.h"
#include "game_player_control.h"
#include "grey_assert.h"
#include "grey_ecs.h"
#include "grey_input.h"
#include "raylib.h"
#include "systems/grey_systems.h"

#include <string.h>
#include <threads.h>

u64 grey_game_asset_path_len = 0;
char *grey_game_asset_path = NULL;
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
  grey_game_asset_path = 0;
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

int main(void) {
  InitWindow(800, 450, "Grey Engine: M3");
  ToggleFullscreen();
  SetTargetFPS(60);

  grey_input_init();
  asset_path_init();

  Arena arena = arena_create(1024 * 1024);

  EcsRegistry reg = ecs_create(arena);
  Texture2D player_sprite = LoadTexture(asset_path("character.png"));

  Entity player = ecs_create_entity(reg);

  ecs_add_position(reg, player, 400, 200);
  ecs_add_render(reg, player, 64, 64, BLUE);
  ecs_add_player(reg, player);
  ecs_add_sprite(reg, player, player_sprite, (Rectangle){0, 0, 16, 24},
                 (SpriteSize){64, 64}, WHITE);
  ecs_add_velocity(reg, player);
  AnimClip anim[] = {
      [IDLE_DOWN] = {.frame_time = 1,
                     .max_frame = 1,
                     .start_x = 0,
                     .start_y = 4},
      [IDLE_RIGHT] = {.frame_time = 1,
                      .max_frame = 1,
                      .start_x = 0,
                      .start_y = 36},
      [IDLE_UP] = {.frame_time = 1,
                   .max_frame = 1,
                   .start_x = 0,
                   .start_y = 68},
      [IDLE_LEFT] = {.frame_time = 1,
                     .max_frame = 1,
                     .start_x = 0,
                     .start_y = 100},
      [WALK_DOWN] = {.frame_time = 0.10f,
                     .max_frame = 4,
                     .start_x = 0,
                     .start_y = 4},
      [WALK_RIGHT] = {.frame_time = 0.10f,
                      .max_frame = 4,
                      .start_x = 0,
                      .start_y = 36},
      [WALK_UP] = {.frame_time = 0.10f,
                   .max_frame = 4,
                   .start_x = 0,
                   .start_y = 68},
      [WALK_LEFT] = {.frame_time = 0.10f,
                     .max_frame = 4,
                     .start_x = 0,
                     .start_y = 100},
  };

  ecs_add_animator(reg, player, anim, IDLE_DOWN);

  while (!WindowShouldClose()) {
    grey_input_begin_frame();
    grey_input_update();

    game_player_control(reg);

    grey_sys_physics_update(reg);
    grey_sys_animator_update(reg);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    grey_sys_render_draw(reg);

    grey_input_draw_gamepad();

    EndDrawing();
  }
  UnloadTexture(player_sprite);
  asset_path_destroy();
  CloseWindow();
  return 0;
}
