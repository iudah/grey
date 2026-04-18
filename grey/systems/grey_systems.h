#ifndef GREY_SYSTEMS_H
#define GREY_SYSTEMS_H

#include "grey_ecs.h"

void grey_sys_player_control(EcsRegistry reg);
void grey_sys_render_draw(EcsRegistry reg);
void grey_sys_physics_update(EcsRegistry reg);

#endif
