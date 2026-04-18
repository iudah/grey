#include "grey_assert.h"
#include "grey_ecs.h"
#include "grey_input.h"

#define SYS_PLAYER_MASK (COMP_PLAYER | COMP_VELOCITY)

void grey_sys_player_control(EcsRegistry reg) {
  GREY_ASSERT(reg, "Null registry.");

  u32 n_entities = ecs_get_number_of_entities(reg);
  for (u32 i = 1; i <= n_entities; ++i) {
    Entity e = i;
    if ((ecs_get_mask(reg, e) & SYS_PLAYER_MASK) == SYS_PLAYER_MASK) {
      VelocityComponent *vel = ecs_get_velocity(reg, e);

      if (grey_input_is_down(ACTION_UP))
        vel->y = -600;
      else if (grey_input_is_down(ACTION_DOWN))
        vel->y = 600;
      else
        vel->y = 0;
      if (grey_input_is_down(ACTION_RIGHT))
        vel->x = 600;
      else if (grey_input_is_down(ACTION_LEFT))
        vel->x = -600;
      else
        vel->x = 0;
    }
  }
}
