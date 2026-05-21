#include "grey_assert.h"
#include "grey_ecs.h"
#include "grey_events.h"
#include "grey_tilemap.h"
#include "raylib.h"
#include "type_alias.h"

#define SYS_PHYSICS_MASK (COMP_POSITION | COMP_VELOCITY)
#define SYS_COLLIDER_MASK (COMP_POSITION | COMP_COLLIDER)

static inline bool aabb_collide(f32 x_1, f32 y_1, f32 w_1, f32 h_1, f32 x_2,
                                f32 y_2, f32 w_2, f32 h_2) {
  return (bool)(x_1 < x_2 + w_2 && x_1 + w_1 > x_2 && y_1 < y_2 + h_2 &&
                y_1 + h_1 > y_2);
}

bool check_tilemap_collision(GreyTileMap *map, Vector2 next,
                             ColliderComponent *col, TileDef *tile_set,
                             GreyEvents event_system) {
  if (!map || !map->grid)
    return false;

  bool hit = false;

#define GAP (0.1f)
  auto left = next.x + col->offset_x;
  auto right = left + col->width;
  auto top = next.y + col->offset_y;
  auto bottom = top + col->height - GAP;

  auto grid_l = (i32)(left / map->tile_size);
  auto grid_r = (i32)(right / map->tile_size);
  auto grid_t = (i32)(top / map->tile_size);
  auto grid_b = (i32)(bottom / map->tile_size);

  if (grid_l >= 0 && grid_r < map->grid_width && grid_t >= 0 &&
      grid_b < map->grid_height) {

    auto grid_width = map->grid_width;
    u32 grid[] = {
        (grid_t * grid_width) + grid_l,
        (grid_b * grid_width) + grid_l,
        (grid_t * grid_width) + grid_r,
        (grid_b * grid_width) + grid_r,
    };
    u32 ids[] = {
        map->grid[grid[0]],
        map->grid[grid[1]],
        map->grid[grid[2]],
        map->grid[grid[3]],
    };

    for (u32 i = 0; i < 4; ++i) {
      if (tile_set[ids[i]].trigger) {
        GreyEvent event = {
            .type = TRIGGER_EVENT,
            .as.trigger = {.trigger_id = tile_set[ids[i]].trigger_id,
                           .map_grid_idx = grid[i]}};
        event_system_add_event(event_system, event);
      }

      if (tile_set[ids[i]].solid) {
        hit = (bool)((int)hit || (int)true);
      }
    }

  } else {
    return true;
  }

  return hit;
}

bool check_ecs_collision(EcsRegistry reg, Entity entity, ColliderComponent *col,
                         Vector2 next, u32 n_entities,
                         GreyEvents event_system) {
  bool hit = false;

  for (u32 j = 1; j <= n_entities; ++j) {
    Entity other = j;
    if (other == entity)
      continue;

    if ((ecs_get_mask(reg, other) & SYS_COLLIDER_MASK) != SYS_COLLIDER_MASK) {
      continue;
    }

    PositionComponent *pos1 = ecs_get_position(reg, other);
    ColliderComponent *col1 = ecs_get_collider(reg, other);

    bool is_trigger = col1->trigger_type != 0;

    f32 x_1 = next.x + col->offset_x;
    f32 y_1 = next.y + col->offset_y;

    f32 x_2 = pos1->x + col1->offset_x;
    f32 y_2 = pos1->y + col1->offset_y;

    if (aabb_collide(x_1, y_1, col->width, col->height, //
                     x_2, y_2, col1->width, col1->height)) {
      if (is_trigger) {
        GreyEvent event = {.type = TRIGGER_EVENT,
                           .as.trigger = {
                               .trigger_id = col1->trigger_type,
                               .entity = entity,
                               .other = other,
                           }};
        event_system_add_event(event_system, event);
        continue;
      }
      hit = (bool)((int)hit || (int)true);
    }
  }

  return hit;
}

void grey_sys_physics_update(EcsRegistry reg, GreyTileMap *map,
                             TileDef *tile_set, GreyEvents event_system) {
  GREY_ASSERT(reg, "Null registry");

  f32 delta_time = GetFrameTime();
  u32 n_entities = ecs_get_number_of_entities(reg);
  for (u32 i = 1; i <= n_entities; ++i) {
    Entity entity = i;
    ComponentMask mask = ecs_get_mask(reg, entity);
    bool has_physics = (mask & SYS_PHYSICS_MASK) == SYS_PHYSICS_MASK;
    bool has_collider = (mask & SYS_COLLIDER_MASK) == SYS_COLLIDER_MASK;

    if ((int)has_collider || (int)has_physics) {
      PositionComponent *pos = ecs_get_position(reg, entity);
      if ((int)has_collider && (int)has_physics) {
        ColliderComponent *col = ecs_get_collider(reg, entity);
        VelocityComponent *vel = ecs_get_velocity(reg, entity);

        Vector2 next = {pos->x + (vel->x * delta_time), pos->y};

        bool hit_x =
            check_ecs_collision(reg, entity, col, next, n_entities,
                                event_system) ||
            check_tilemap_collision(map, next, col, tile_set, event_system);

        if (hit_x) {
          next.x = pos->x;
          vel->x = 0;
        }

        next.y = pos->y + (vel->y * delta_time);
        bool hit_y =
            check_tilemap_collision(map, next, col, tile_set, event_system) ||
            check_ecs_collision(reg, entity, col, next, n_entities,
                                event_system);

        if (hit_y) {
          next.y = pos->y;
          vel->y = 0;
        }

        pos->x = next.x;
        pos->y = next.y;

      } else if (has_physics) {
        VelocityComponent *vel = ecs_get_velocity(reg, entity);

        pos->x += vel->x * delta_time;
        pos->y += vel->y * delta_time;
      }
    }
  }
}
