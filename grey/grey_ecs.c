#include "grey_ecs.h"
#include <stdint.h>
#include <string.h>

struct ecs_registry {
  RenderComponent *render;
  PositionComponent *positions;
  u64 *masks;
  u32 number_of_entities;
};

static inline bool is_valid_entity(EcsRegistry reg, Entity e) {
  return reg && e >= 1 && e <= reg->number_of_entities;
}

static inline bool has_component(EcsRegistry reg, Entity e,
                                 ComponentMask mask) {
  if (!is_valid_entity(reg, e))
    return false;

  u64 comps = reg->masks[e - 1];
  return (comps & mask) != 0;
}

EcsRegistry ecs_create(Arena arena) {
  if (!arena)
    return NULL;

  EcsRegistry reg = arena_allocate(arena, sizeof(*reg));

  if (!reg)
    return NULL;

  reg->positions =
      arena_allocate(arena, MAX_ENTITIES * sizeof(PositionComponent));
  reg->render = arena_allocate(arena, MAX_ENTITIES * sizeof(RenderComponent));
  reg->masks = arena_allocate(arena, MAX_ENTITIES * sizeof(u64));
  reg->number_of_entities = 0;

  if (!reg->positions || !reg->render || !reg->masks) {
    return NULL;
  }

  memset(reg->masks, 0, MAX_ENTITIES * sizeof(u64));

  return reg;
}

Entity ecs_create_entity(EcsRegistry reg) {
  if (!reg || reg->number_of_entities >= MAX_ENTITIES)
    return 0;
  return ++reg->number_of_entities;
}

void ecs_destroy_entity(EcsRegistry reg, Entity e) {
  if (!is_valid_entity(reg, e))
    return;

  reg->masks[e - 1] = 0;
}

void ecs_add_position(EcsRegistry reg, Entity e, f32 x, f32 y) {
  if (!is_valid_entity(reg, e))
    return;

  reg->masks[e - 1] |= COMP_POSITION;
  reg->positions[e - 1] = (PositionComponent){x, y};
}

void ecs_add_render(EcsRegistry reg, Entity e, f32 w, f32 h, Color color) {
  if (!is_valid_entity(reg, e))
    return;

  reg->masks[e - 1] |= COMP_RENDER;
  reg->render[e - 1] = (RenderComponent){w, h, color};
}

PositionComponent *ecs_get_position(EcsRegistry reg, Entity e) {
  if (!has_component(reg, e, COMP_POSITION))
    return NULL;
  return &reg->positions[e - 1];
}

RenderComponent *ecs_get_render(EcsRegistry reg, Entity e) {
  if (!has_component(reg, e, COMP_RENDER))
    return NULL;
  return &reg->render[e - 1];
}
