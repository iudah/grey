#include "grey_ecs.h"
#include "arena.h"
#include "grey_assert.h"
#include <stdint.h>
#include <string.h>

struct ecs_registry {
  RenderComponent *render;
  PositionComponent *positions;
  SpriteComponent *sprites;
  VelocityComponent *velocities;
  AnimatorComponent *animators;
  u64 *masks;
  u32 number_of_entities;
};

static inline bool is_valid_entity(EcsRegistry reg, Entity e) {
  GREY_ASSERT(reg, "Null registry passed.");
  GREY_ASSERT(e >= 1 && e <= reg->number_of_entities, "Invalid entity.");
  return e >= 1 && e <= reg->number_of_entities;
}

static inline bool has_component(EcsRegistry reg, Entity e,
                                 ComponentMask mask) {
  if (!is_valid_entity(reg, e))
    return false;

  u64 comps = reg->masks[e - 1];
  return (comps & mask) != 0;
}

EcsRegistry ecs_create(Arena arena) {
  GREY_ASSERT(arena, "NULL arena passed.");

  EcsRegistry reg = arena_allocate(arena, sizeof(*reg));

  if (!reg)
    return NULL;

  reg->positions =
      arena_allocate(arena, MAX_ENTITIES * sizeof(PositionComponent));
  reg->render = arena_allocate(arena, MAX_ENTITIES * sizeof(RenderComponent));
  reg->sprites = arena_allocate(arena, MAX_ENTITIES * sizeof(SpriteComponent));
  reg->velocities =
      arena_allocate(arena, MAX_ENTITIES * sizeof(VelocityComponent));
  reg->animators =
      arena_allocate(arena, MAX_ENTITIES * sizeof(AnimatorComponent));

  reg->masks = arena_allocate(arena, MAX_ENTITIES * sizeof(u64));
  reg->number_of_entities = 0;

  GREY_ASSERT(reg->positions && reg->render && reg->masks && reg->sprites,
              "Arena has insufficient memory");

  memset(reg->masks, 0, MAX_ENTITIES * sizeof(u64));

  return reg;
}

Entity ecs_create_entity(EcsRegistry reg) {
  GREY_ASSERT(reg, "Null registry.");
  if (reg->number_of_entities >= MAX_ENTITIES)
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

void ecs_add_player(EcsRegistry reg, Entity e) {
  if (!is_valid_entity(reg, e))
    return;

  reg->masks[e - 1] |= COMP_PLAYER;
}

void ecs_add_sprite(EcsRegistry reg, Entity e, Texture2D texture, Rectangle src,
                    SpriteSize dest, Color tint) {
  if (!is_valid_entity(reg, e))
    return;

  reg->masks[e - 1] |= COMP_SPRITE;
  reg->sprites[e - 1] = (SpriteComponent){
      .texture = texture, .src = src, .tint = tint, .dest = dest};
}

void ecs_add_velocity(EcsRegistry reg, Entity e) {
  if (!is_valid_entity(reg, e))
    return;

  reg->masks[e - 1] |= COMP_VELOCITY;
  reg->velocities[e - 1] = (VelocityComponent){0, 0};
}

void ecs_add_animator(EcsRegistry reg, Entity e, f32 frame_time,
                      u32 tile_index_x, u32 tile_index_y, u8 max_frame) {

  if (!is_valid_entity(reg, e))
    return;

  reg->masks[e - 1] |= COMP_ANIMATOR;
  reg->animators[e - 1] = (AnimatorComponent){
      0, frame_time, tile_index_x, tile_index_y, max_frame, 0};
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

SpriteComponent *ecs_get_sprite(EcsRegistry reg, Entity e) {
  if (!has_component(reg, e, COMP_SPRITE))
    return NULL;
  return &reg->sprites[e - 1];
}

VelocityComponent *ecs_get_velocity(EcsRegistry reg, Entity e) {
  if (!has_component(reg, e, COMP_VELOCITY))
    return NULL;
  return &reg->velocities[e - 1];
}

AnimatorComponent *ecs_get_animator(EcsRegistry reg, Entity e) {
  if (!has_component(reg, e, COMP_ANIMATOR))
    return NULL;
  return &reg->animators[e - 1];
}

u64 ecs_get_mask(EcsRegistry reg, Entity e) {
  if (!is_valid_entity(reg, e))
    return 0;
  return reg->masks[e - 1];
}

u32 ecs_get_number_of_entities(EcsRegistry reg) {
  GREY_ASSERT(reg, "Null registry.");
  return reg->number_of_entities;
}
