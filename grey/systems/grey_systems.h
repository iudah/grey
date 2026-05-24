#ifndef GREY_SYSTEMS_H
#define GREY_SYSTEMS_H

#include "grey_ecs.h"
#include "grey_events.h"
#include "grey_tilemap.h"
#include "type_alias.h"

void grey_sys_render_draw(EcsRegistry reg);
void grey_sys_physics_update(EcsRegistry reg, GreyTileMap *map,
                             TileDef *tile_set, GreyEvents event_system);
void grey_sys_animator_update(EcsRegistry reg);
void grey_sys_camera_update(EcsRegistry reg, Entity camera_ntt,
                            Camera2D *camera);

void grey_set_camera_trauma(EcsRegistry reg, Entity camera, f32 trauma_pxs);

#endif
