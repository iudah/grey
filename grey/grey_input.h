#ifndef GREY_INPUT_H
#define GREY_INPUT_H

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

void grey_input_init();

void grey_input_update();
void grey_input_begin_frame();

bool grey_input_is_down(GreyAction action);
bool grey_input_is_pressed(GreyAction action);
bool grey_input_is_released(GreyAction action);


void grey_input_draw_gamepad() ;

#endif