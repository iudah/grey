#include "grey_assert.h"
#include "grey_ecs.h"
#include "raylib.h"
#include "type_alias.h"

#define SYS_PHYSICS_MASK (COMP_POSITION | COMP_VELOCITY)
#define SYS_COLLIDER_MASK (COMP_POSITION | COMP_COLLIDER)

static inline bool aabb_collide(f32 x1, f32 y1, f32 w1, f32 h1, f32 x2, f32 y2,
                                f32 w2, f32 h2) {
  return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

void grey_sys_physics_update(EcsRegistry reg) {
  GREY_ASSERT(reg, "Null registry");

  f32 dt = GetFrameTime();
  u32 n_entities = ecs_get_number_of_entities(reg);
  for (u32 i = 1; i <= n_entities; ++i) {
    Entity e = i;
    ComponentMask mask = ecs_get_mask(reg, e);
    bool has_physics = (mask & SYS_PHYSICS_MASK) == SYS_PHYSICS_MASK;
    bool has_collider = (mask & SYS_COLLIDER_MASK) == SYS_COLLIDER_MASK;

    if (has_collider || has_physics) {
      PositionComponent *pos = ecs_get_position(reg, e);
      if (has_collider && has_physics) {
        ColliderComponent *col = ecs_get_collider(reg, e);
        VelocityComponent *vel = ecs_get_velocity(reg, e);

        f32 next_x = pos->x + vel->x * dt;
        f32 next_y = pos->y + vel->y * dt;

        bool hit_x = false;
        for (u32 j = 1; j <= n_entities; ++j) {
          Entity f = j;
          if (f == e)
            continue;

          if ((ecs_get_mask(reg, f) & SYS_COLLIDER_MASK) != SYS_COLLIDER_MASK) {
            continue;
          }

          PositionComponent *pos1 = ecs_get_position(reg, f);
          ColliderComponent *col1 = ecs_get_collider(reg, f);

          f32 x1 = next_x + col->offset_x;
          f32 y1 = pos->y + col->offset_y;

          f32 x2 = pos1->x + col1->offset_x;
          f32 y2 = pos1->y + col1->offset_y;

          if (aabb_collide(x1, y1, col->width, col->height, //
                           x2, y2, col1->width, col1->height)) {
            hit_x = true;
            break;
          }
        }

        if (hit_x) {
          next_x = pos->x;
          vel->x = 0;
        }

        bool hit_y = false;
        for (u32 j = 1; j <= n_entities; ++j) {
          Entity f = j;
          if (f == e)
            continue;

          if ((ecs_get_mask(reg, f) & SYS_COLLIDER_MASK) != SYS_COLLIDER_MASK) {
            continue;
          }

          PositionComponent *pos1 = ecs_get_position(reg, f);
          ColliderComponent *col1 = ecs_get_collider(reg, f);

          f32 x1 = next_x + col->offset_x;
          f32 y1 = next_y + col->offset_y;

          f32 x2 = pos1->x + col1->offset_x;
          f32 y2 = pos1->y + col1->offset_y;

          if (aabb_collide(x1, y1, col->width, col->height, //
                           x2, y2, col1->width, col1->height)) {
            hit_y = true;
            break;
          }
        }

        if (hit_y) {
          next_y = pos->y;
          vel->y = 0;
        }

        pos->x = next_x;
        pos->y = next_y;

      } else if (has_physics) {
        VelocityComponent *vel = ecs_get_velocity(reg, e);

        pos->x += vel->x * dt;
        pos->y += vel->y * dt;
      }
    }
  }
}
