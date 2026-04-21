#include "grey_assert.h"
#include "grey_ecs.h"
#include "raylib.h"

#define SYS_RENDER_MASK (COMP_POSITION | COMP_RENDER)
#define SYS_SPRITE_MASK (COMP_POSITION | COMP_SPRITE)

void grey_sys_render_draw(EcsRegistry reg) {
  GREY_ASSERT(reg, "Null registry");

  u32 n_entities = ecs_get_number_of_entities(reg);
  for (u32 i = 0; i < n_entities;) {
    Entity e = ++i;
    ComponentMask mask = ecs_get_mask(reg, e);
    bool has_render = (mask & SYS_RENDER_MASK) == SYS_RENDER_MASK;
    bool has_sprite = (mask & SYS_SPRITE_MASK) == SYS_SPRITE_MASK;

    if (has_sprite || has_render) {
      PositionComponent *pos = ecs_get_position(reg, e);

      if (has_sprite) {
        SpriteComponent *spr = ecs_get_sprite(reg, e);

        DrawTexturePro(
            spr->texture, spr->src,
            (Rectangle){pos->x, pos->y, spr->dest.width, spr->dest.height},
            (Vector2){0, 0}, 0, spr->tint);
      }

      if (has_render) {
        RenderComponent *ren = ecs_get_render(reg, e);

        DrawRectangle(pos->x, pos->y, ren->width, ren->height,
                      Fade(ren->color, 0.15f));
      }
    }
  }
}
