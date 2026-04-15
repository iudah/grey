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
  COMP_PLAYER = 1 << 2 
} ComponentMask;

typedef struct {
  f32 x, y;
} PositionComponent;
typedef struct {
  f32 width, height;
  Color color;
} RenderComponent;

typedef struct ecs_registry *EcsRegistry;

EcsRegistry ecs_create(Arena arena);
Entity ecs_create_entity(EcsRegistry reg);
void ecs_destroy_entity(EcsRegistry reg, Entity e);

void ecs_add_position(EcsRegistry reg, Entity e, f32 x, f32 y);
void ecs_add_render(EcsRegistry reg, Entity e, f32 w, f32 h, Color color);
void ecs_add_player(EcsRegistry reg, Entity e);

u64 ecs_get_mask(EcsRegistry reg, Entity e);
u32 ecs_get_number_of_entities(EcsRegistry reg);
PositionComponent *ecs_get_position(EcsRegistry reg, Entity e);
RenderComponent *ecs_get_render(EcsRegistry reg, Entity e);

#endif

