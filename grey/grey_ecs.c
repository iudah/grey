#include "grey_ecs.h"
#include "arena.h"
#include "grey_assert.h"
#include "raylib.h"
#include <stdint.h>
#include <string.h>

struct ecs_registry {
  RenderComponent *render;
  PositionComponent *positions;
  SpriteComponent *sprites;
  VelocityComponent *velocities;
  AnimatorComponent *animators;
  ColliderComponent *colliders;
  CameraComponent *cameras;
  u64 *masks;
  u32 number_of_entities;
};

static inline bool is_valid_entity(EcsRegistry reg, Entity entity) {
  GREY_ASSERT(reg, "Null registry passed.");
  GREY_ASSERT(entity >= 1 && entity <= reg->number_of_entities,
              "Invalid entity.");
  return entity >= 1 && entity <= reg->number_of_entities;
}

static inline bool has_component(EcsRegistry reg, Entity entity,
                                 ComponentMask mask) {
  if (!is_valid_entity(reg, entity))
    return false;

  u64 comps = reg->masks[entity - 1];
  return (comps & mask) != 0;
}

EcsRegistry ecs_create(Arena arena) {
  GREY_ASSERT(arena, "NULL arena passed.");

  EcsRegistry reg = arena_allocate(arena, sizeof(*reg));

  if (!reg)
    return nullptr;

  reg->positions =
      arena_allocate(arena, MAX_ENTITIES * sizeof(PositionComponent));
  reg->render = arena_allocate(arena, MAX_ENTITIES * sizeof(RenderComponent));
  reg->sprites = arena_allocate(arena, MAX_ENTITIES * sizeof(SpriteComponent));
  reg->velocities =
      arena_allocate(arena, MAX_ENTITIES * sizeof(VelocityComponent));
  reg->animators =
      arena_allocate(arena, MAX_ENTITIES * sizeof(AnimatorComponent));
  reg->colliders =
      arena_allocate(arena, MAX_ENTITIES * sizeof(ColliderComponent));
  reg->cameras = arena_allocate(arena, MAX_ENTITIES * sizeof(CameraComponent));

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

void ecs_destroy_entity(EcsRegistry reg, Entity entity) {
  if (!is_valid_entity(reg, entity))
    return;

  reg->masks[entity - 1] = 0;
}

void ecs_add_position(EcsRegistry reg, Entity entity, f32 x_pos, f32 y_pos) {
  if (!is_valid_entity(reg, entity))
    return;

  reg->masks[entity - 1] |= COMP_POSITION;
  reg->positions[entity - 1] = (PositionComponent){x_pos, y_pos};
}

void ecs_add_render(EcsRegistry reg, Entity entity, f32 width, f32 height,
                    Color color) {
  if (!is_valid_entity(reg, entity))
    return;

  reg->masks[entity - 1] |= COMP_RENDER;
  reg->render[entity - 1] = (RenderComponent){width, height, color};
}

void ecs_add_player(EcsRegistry reg, Entity entity) {
  if (!is_valid_entity(reg, entity))
    return;

  reg->masks[entity - 1] |= COMP_PLAYER;
}

void ecs_add_sprite(EcsRegistry reg, Entity entity, Texture2D texture,
                    Rectangle src, SpriteSize dest, Color tint) {
  if (!is_valid_entity(reg, entity))
    return;

  reg->masks[entity - 1] |= COMP_SPRITE;
  reg->sprites[entity - 1] = (SpriteComponent){
      .texture = texture, .src = src, .tint = tint, .dest = dest};
}

void ecs_add_velocity(EcsRegistry reg, Entity entity) {
  if (!is_valid_entity(reg, entity))
    return;

  reg->masks[entity - 1] |= COMP_VELOCITY;
  reg->velocities[entity - 1] = (VelocityComponent){0, 0};
}

void ecs_add_animator(EcsRegistry reg, Entity entity, AnimClip *action_anims,
                      u8 initial_clip_id) {
  if (!is_valid_entity(reg, entity))
    return;

  reg->masks[entity - 1] |= COMP_ANIMATOR;
  reg->animators[entity - 1] =
      (AnimatorComponent){action_anims, 0, initial_clip_id, 0};
}

void ecs_add_collider(EcsRegistry reg, Entity entity, f32 width, f32 height,
                      f32 offset_x, f32 offset_y, bool is_trigger,
                      u32 trigger_type) {
  if (!is_valid_entity(reg, entity))
    return;

  reg->masks[entity - 1] |= COMP_COLLIDER;
  reg->colliders[entity - 1] = (ColliderComponent){
      width, height, offset_x, offset_y, is_trigger ? trigger_type : 0};
}

void ecs_add_camera(EcsRegistry reg, Entity camera_ntt, Vector2 half_bound,
                    Vector2 target_position, Entity target_entity, f32 speed,
                    Vector2 min, Vector2 max) {
  if (!is_valid_entity(reg, camera_ntt))
    return;

  reg->masks[camera_ntt - 1] |= COMP_CAMERA;
  reg->cameras[camera_ntt - 1] = (CameraComponent){
      half_bound, target_position, min, max, target_entity, speed, 0};
}

PositionComponent *ecs_get_position(EcsRegistry reg, Entity entity) {
  if (!has_component(reg, entity, COMP_POSITION))
    return nullptr;
  return &reg->positions[entity - 1];
}

RenderComponent *ecs_get_render(EcsRegistry reg, Entity entity) {
  if (!has_component(reg, entity, COMP_RENDER))
    return nullptr;
  return &reg->render[entity - 1];
}

SpriteComponent *ecs_get_sprite(EcsRegistry reg, Entity entity) {
  if (!has_component(reg, entity, COMP_SPRITE))
    return nullptr;
  return &reg->sprites[entity - 1];
}

VelocityComponent *ecs_get_velocity(EcsRegistry reg, Entity entity) {
  if (!has_component(reg, entity, COMP_VELOCITY))
    return nullptr;
  return &reg->velocities[entity - 1];
}

AnimatorComponent *ecs_get_animator(EcsRegistry reg, Entity entity) {
  if (!has_component(reg, entity, COMP_ANIMATOR))
    return nullptr;
  return &reg->animators[entity - 1];
}

ColliderComponent *ecs_get_collider(EcsRegistry reg, Entity entity) {
  if (!has_component(reg, entity, COMP_COLLIDER))
    return nullptr;
  return &reg->colliders[entity - 1];
}

CameraComponent *ecs_get_camera(EcsRegistry reg, Entity camera) {
  if (!has_component(reg, camera, COMP_CAMERA))
    return nullptr;
  return &reg->cameras[camera - 1];
}

u64 ecs_get_mask(EcsRegistry reg, Entity entity) {
  if (!is_valid_entity(reg, entity))
    return 0;
  return reg->masks[entity - 1];
}

u32 ecs_get_number_of_entities(EcsRegistry reg) {
  GREY_ASSERT(reg, "Null registry.");
  return reg->number_of_entities;
}
