#ifndef GREY_INPUT_H
#define GREY_INPUT_H

#include "raylib.h"
#include "type_alias.h"
#include <stdbool.h>

typedef enum {
  ACTION_UP,
  ACTION_DOWN,
  ACTION_LEFT,
  ACTION_RIGHT,
  ACTION_A,
  ACTION_O,
  ACTION_X,
  ACTION_D,
  ACTION_MAX
} GreyAction;
typedef enum { GREY_DPAD, GREY_JOYSTICK } GreyPadMode;

void grey_input_init(GreyPadMode virtual_pad_mode);

void grey_input_update();
void grey_input_begin_frame();

bool grey_input_is_down(GreyAction action);
bool grey_input_is_pressed(GreyAction action);
bool grey_input_is_released(GreyAction action);

void grey_input_draw_gamepad();
void grey_set_action_btn(GreyAction action, Vector2 cntr, f32 rad);
void grey_default_action_btn();

#endif
