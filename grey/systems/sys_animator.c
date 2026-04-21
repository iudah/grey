#include "grey_assert.h"
#include "grey_ecs.h"
#include "raylib.h"

#define SYS_ANIMATOR_MASK (COMP_ANIMATOR | COMP_SPRITE)

void grey_sys_animator_update(EcsRegistry reg) {
  GREY_ASSERT(reg, "Null registry");

  u32 n_entities = ecs_get_number_of_entities(reg);
  for (u32 i = 1; i <= n_entities; ++i) {
    Entity e = i;
    ComponentMask mask = ecs_get_mask(reg, e);
    bool has_animator = (mask & SYS_ANIMATOR_MASK) == SYS_ANIMATOR_MASK;

    if (has_animator) {
      AnimatorComponent *anim = ecs_get_animator(reg, e);

      SpriteComponent *spr = ecs_get_sprite(reg, e);

      anim->timer += GetFrameTime();
      if (anim->timer >= anim->frame_time) {
        anim->timer = 0;
        ++anim->current_frame;
        if (anim->current_frame >= anim->max_frame)
          anim->current_frame = 0;
      }

      spr->src.x = anim->current_frame * spr->src.width + anim->start_x;
      spr->src.y = anim->start_y;
    }
  }
}
