#ifndef GREY_ECS_H
#define GREY_ECS_H

#include "arena.h"
#include "raylib.h"
#include "type_alias.h"

#define MAX_ENTITIES (1 << 10)

typedef u32 Entity;

typedef enum {
  COMP_NONE = 0,
  COMP_POSITION = 1 << 0,
  COMP_RENDER = 1 << 1,
  COMP_PLAYER = 1 << 2,
  COMP_SPRITE = 1 << 3,
  COMP_VELOCITY = 1 << 4,
  COMP_ANIMATOR = 1 << 5,
  COMP_COLLIDER = 1 << 6,
  COMP_CAMERA = 1 << 7
} ComponentMask;

typedef struct {
  f32 x, y;
} PositionComponent;
typedef struct {
  f32 width, height;
  Color color;
} RenderComponent;
typedef struct {
  f32 width, height;
} SpriteSize;
typedef struct {
  Texture2D texture;
  Rectangle src;
  SpriteSize dest;
  Color tint;
} SpriteComponent;
typedef PositionComponent VelocityComponent;
typedef struct {
  f32 frame_time;
  u32 start_x;
  u32 start_y;
  u8 max_frame;
} AnimClip;
typedef struct {
  AnimClip *action_clips;
  f32 timer;
  u8 active_clip_id;
  u8 current_frame;
} AnimatorComponent;
typedef struct {
  f32 width;
  f32 height;
  f32 offset_x;
  f32 offset_y;
  u32 trigger_type;
} ColliderComponent;
typedef struct {
  Vector2 half_bound;
  Vector2 target;
  Entity target_entity;
  f32 speed;
  f32 shake_trauma;
} CameraComponent;
typedef struct ecs_registry *EcsRegistry;

EcsRegistry ecs_create(Arena arena);
Entity ecs_create_entity(EcsRegistry reg);
void ecs_destroy_entity(EcsRegistry reg, Entity entity);

void ecs_add_position(EcsRegistry reg, Entity entity, f32 x_pos, f32 y_pos);
void ecs_add_render(EcsRegistry reg, Entity e, f32 w, f32 h, Color color);
void ecs_add_player(EcsRegistry reg, Entity e);
void ecs_add_sprite(EcsRegistry reg, Entity e, Texture2D texture, Rectangle src,
                    SpriteSize dest, Color tint);
void ecs_add_velocity(EcsRegistry reg, Entity e);
void ecs_add_animator(EcsRegistry reg, Entity e, AnimClip *action_anims,
                      u8 initial_clip_id);
void ecs_add_collider(EcsRegistry reg, Entity e, f32 width, f32 height,
                      f32 offset_x, f32 offset_y, bool is_trigger,
                      u32 trigger_type);
void ecs_add_camera(EcsRegistry reg, Entity camera_ntt, Vector2 half_bound,
                    Vector2 target_position, Entity target_entity, f32 speed,
                    f32 shake_trauma);

u64 ecs_get_mask(EcsRegistry reg, Entity e);
u32 ecs_get_number_of_entities(EcsRegistry reg);
PositionComponent *ecs_get_position(EcsRegistry reg, Entity e);
RenderComponent *ecs_get_render(EcsRegistry reg, Entity e);
SpriteComponent *ecs_get_sprite(EcsRegistry reg, Entity e);
VelocityComponent *ecs_get_velocity(EcsRegistry reg, Entity e);
AnimatorComponent *ecs_get_animator(EcsRegistry reg, Entity e);
ColliderComponent *ecs_get_collider(EcsRegistry reg, Entity e);
CameraComponent *ecs_get_camera(EcsRegistry reg, Entity camera);

#endif
