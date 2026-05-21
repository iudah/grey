#ifndef GAME_ANIM_H
#define GAME_ANIM_H

typedef enum {
  IDLE_UP,
  IDLE_DOWN,
  IDLE_LEFT,
  IDLE_RIGHT,
  WALK_UP,
  WALK_DOWN,
  WALK_LEFT,
  WALK_RIGHT
} AnimId;

typedef enum {
  WATER_TILE = 1,
  ONE_WAY_ENTRY_TRIGGER,
  ONE_WAY_EXIT_TRIGGER,
} GameTriggers;

#endif
