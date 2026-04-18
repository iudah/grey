#include "grey_assert.h"
#include "grey_ecs.h"
#include "raylib.h"

#define SYS_PHYSICS_MASK (COMP_POSITION | COMP_VELOCITY)

void grey_sys_physics_update(EcsRegistry reg) {
  GREY_ASSERT(reg, "Null registry");

  f32 dt = GetFrameTime();
  u32 n_entities = ecs_get_number_of_entities(reg);
  for (u32 i = 1; i <= n_entities; ++i) {
    Entity e = i;
    ComponentMask mask = ecs_get_mask(reg, e);
    bool has_physics = (mask & SYS_PHYSICS_MASK) == SYS_PHYSICS_MASK;

    if (has_physics) {
      PositionComponent *pos = ecs_get_position(reg, e);
      VelocityComponent *vel = ecs_get_velocity(reg, e);

      pos->x += vel->x * dt;
      pos->y += vel->y * dt;
    }
  }
}
