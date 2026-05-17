#include "grey_assert.h"
#include "grey_ecs.h"
#include "raylib.h"

#define SYS_ANIMATOR_MASK (COMP_ANIMATOR | COMP_SPRITE)

void grey_sys_animator_update(EcsRegistry reg) {
  GREY_ASSERT(reg, "Null registry");

  u32 n_entities = ecs_get_number_of_entities(reg);
  for (u32 i = 1; i <= n_entities; ++i) {
    Entity entity = i;
    ComponentMask mask = ecs_get_mask(reg, entity);
    bool has_animator = (mask & SYS_ANIMATOR_MASK) == SYS_ANIMATOR_MASK;

    if (has_animator) {
      AnimatorComponent *anim = ecs_get_animator(reg, entity);
      AnimClip clip = anim->action_clips[anim->active_clip_id];
      SpriteComponent *spr = ecs_get_sprite(reg, entity);

      if (anim->current_frame >= clip.max_frame)
        anim->current_frame = 0;

      anim->timer += GetFrameTime();
      if (anim->timer >= clip.frame_time) {
        anim->timer = 0;
        ++anim->current_frame;
      }

      spr->src.x = (anim->current_frame * spr->src.width) + clip.start_x;
      spr->src.y = clip.start_y;
    }
  }
}
