#include "arena.h"
#include "grey_ecs.h"
#include "grey_input.h"
#include "raylib.h"

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

  Color c = RED;

  while (!WindowShouldClose()) {
    grey_input_begin_frame();
    grey_input_update();

    PositionComponent *position = ecs_get_position(reg, player);

    if (grey_input_is_down(ACTION_LEFT)) {
      position->x -= 10;
    }
    if (grey_input_is_down(ACTION_RIGHT)) {
      position->x += 10;
    }
    if (grey_input_is_down(ACTION_UP)) {
      position->y -= 10;
    }
    if (grey_input_is_down(ACTION_DOWN)) {
      position->y += 10;
    }

    RenderComponent *render = ecs_get_render(reg, player);
    if (grey_input_is_pressed(ACTION_A)) {
      Color tmp = render->color;
      render->color = c;
      c = tmp;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawRectangle(position->x, position->y, render->width, render->height,
                  render->color);

    grey_input_draw_gamepad();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
