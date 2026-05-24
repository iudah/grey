#ifndef GAME_PLAYER_CONTROL_H
#define GAME_PLAYER_CONTROL_H

#include "grey_ecs.h"
#include "grey_events.h"

void game_player_control(EcsRegistry reg);
void game_system_trigger_update(GreyEvents system, EcsRegistry reg,
                                Entity camera_ntt, u8 **trigger_cell);

#endif
