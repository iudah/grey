#include "game_anim.h"
#include "grey_assert.h"
#include "grey_ecs.h"
#include "grey_input.h"

#define SYS_PLAYER_MASK (COMP_PLAYER | COMP_VELOCITY)

void game_player_control(EcsRegistry reg) {
  GREY_ASSERT(reg, "Null registry.");

  static AnimId last_anim = 0;

  u32 n_entities = ecs_get_number_of_entities(reg);
  for (u32 i = 1; i <= n_entities; ++i) {
    Entity e = i;
    if ((ecs_get_mask(reg, e) & SYS_PLAYER_MASK) == SYS_PLAYER_MASK) {
      VelocityComponent *vel = ecs_get_velocity(reg, e);
      AnimatorComponent *anim = ecs_get_animator(reg, e);

      last_anim = anim->active_clip_id;

      if (grey_input_is_down(ACTION_RIGHT)) {
        vel->x = 60;
        anim->active_clip_id = WALK_RIGHT;
      } else if (grey_input_is_down(ACTION_LEFT)) {
        vel->x = -60;
        anim->active_clip_id = WALK_LEFT;
      } else if (grey_input_is_down(ACTION_UP)) {
        vel->y = -60;
        anim->active_clip_id = WALK_UP;
      } else if (grey_input_is_down(ACTION_DOWN)) {
        vel->y = 60;
        anim->active_clip_id = WALK_DOWN;
      } else {
        vel->x = 0;
        vel->y = 0;
        if (last_anim == WALK_UP || last_anim == IDLE_UP)
          anim->active_clip_id = IDLE_UP;
        else if (last_anim == WALK_DOWN || last_anim == IDLE_DOWN)
          anim->active_clip_id = IDLE_DOWN;
        else if (last_anim == WALK_LEFT || last_anim == IDLE_LEFT)
          anim->active_clip_id = IDLE_LEFT;
        else if (last_anim == WALK_RIGHT || last_anim == IDLE_RIGHT)
          anim->active_clip_id = IDLE_RIGHT;
      }
    }
  }
}
