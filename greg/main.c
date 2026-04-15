#include "arena.h"
#include "grey_ecs.h"
#include "grey_input.h"
#include "raylib.h"
#include "systems/grey_systems.h"

int main(void) {
  InitWindow(800, 450, "Grey Engine: M1");
  ToggleFullscreen();
  SetTargetFPS(60);

  grey_input_init();

  Arena arena = arena_create(1024 * 1024);

  EcsRegistry reg = ecs_create(arena);
  Entity player = ecs_create_entity(reg);

  ecs_add_position(reg, player, 400, 200);
  ecs_add_render(reg, player, 32, 32, BLUE);
  ecs_add_player(reg, player);

  Color c = RED;

  while (!WindowShouldClose()) {
    grey_input_begin_frame();
    grey_input_update();

    grey_sys_player_control(reg);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    grey_sys_render_draw(reg);

    grey_input_draw_gamepad();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
