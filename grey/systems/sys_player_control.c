#include "grey_ecs.h"
#include "grey_input.h"

#define SYS_PLAYER_MASK (COMP_PLAYER | COMP_POSITION)

void grey_sys_player_control(EcsRegistry reg) {
  u32 n_entities = ecs_get_number_of_entities(reg);
  for (u32 i = 0; i < n_entities;) {
    Entity e = ++i;
    if ((ecs_get_mask(reg, e) & SYS_PLAYER_MASK) == SYS_PLAYER_MASK) {
      PositionComponent *pos = ecs_get_position(reg, e);

      if (grey_input_is_down(ACTION_UP))
        pos->y -= 10;
      if (grey_input_is_down(ACTION_DOWN))
        pos->y += 10;
      if (grey_input_is_down(ACTION_RIGHT))
        pos->x += 10;
      if (grey_input_is_down(ACTION_LEFT))
        pos->x -= 10;
    }
  }
}
