#include "grey_input.h"
#include "raylib.h"
#include "type_alias.h"
#include <stdbool.h>

typedef u8 VirtualKey;
typedef enum {
  GREY_KEY_DOWN = 1 << 0,
  GREY_KEY_PRESSED = 1 << 1,
  GREY_KEY_RELEASED = 1 << 2
} VirtualKeyAction;

VirtualKey keys[ACTION_MAX] = {0};
Rectangle button_area[ACTION_MAX];

i32 key_map[ACTION_MAX] = {
    [ACTION_UP] = KEY_UP,     [ACTION_DOWN] = KEY_DOWN,
    [ACTION_LEFT] = KEY_LEFT, [ACTION_RIGHT] = KEY_RIGHT,
    [ACTION_A] = KEY_W,       [ACTION_O] = KEY_DOWN,
    [ACTION_X] = KEY_S,       [ACTION_D] = KEY_A,
};

i32 pad_map[ACTION_MAX] = {
    [ACTION_UP] = GAMEPAD_BUTTON_LEFT_FACE_UP,
    [ACTION_DOWN] = GAMEPAD_BUTTON_LEFT_FACE_DOWN,
    [ACTION_LEFT] = GAMEPAD_BUTTON_LEFT_FACE_LEFT,
    [ACTION_RIGHT] = GAMEPAD_BUTTON_LEFT_FACE_RIGHT,
    [ACTION_A] = GAMEPAD_BUTTON_RIGHT_FACE_UP,
    [ACTION_O] = GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,
    [ACTION_X] = GAMEPAD_BUTTON_RIGHT_FACE_DOWN,
    [ACTION_D] = GAMEPAD_BUTTON_RIGHT_FACE_LEFT,
};

static inline void grey_touch_input_init() {
  // Load button positions from asset/button.*
  f32 sw = (f32)GetScreenWidth();
  f32 sh = (f32)GetScreenHeight();

  f32 btn_w = 0.15f * sw;
  f32 btn_h = 0.20f * sh;

  f32 pad_base_x = 0.05f * sw;
  f32 pad_base_y = 0.50f * sh;

  button_area[ACTION_LEFT] =
      (Rectangle){pad_base_x, pad_base_y + btn_h, btn_w, btn_h};
  button_area[ACTION_RIGHT] =
      (Rectangle){pad_base_x + (btn_w * 2), pad_base_y + btn_h, btn_w, btn_h};
  button_area[ACTION_UP] =
      (Rectangle){pad_base_x + btn_w, pad_base_y, btn_w, btn_h};
  button_area[ACTION_DOWN] =
      (Rectangle){pad_base_x + btn_w, pad_base_y + (btn_h * 2), btn_w, btn_h};

  button_area[ACTION_A] =
      (Rectangle){sw * 0.75f, pad_base_y + btn_h, btn_w * 1.5f, btn_h * 1.5f};
}

void grey_input_init() {
#ifdef GREY_USE_TOUCH
  grey_touch_input_init();
#endif
}

static inline bool key_down(GreyAction k) {
  return (keys[k] & GREY_KEY_DOWN) != 0;
}

static inline bool key_pressed(GreyAction k) {
  return (keys[k] & GREY_KEY_PRESSED) != 0;
}

static inline bool key_released(GreyAction k) {
  return (keys[k] & GREY_KEY_RELEASED) != 0;
}

static inline void key_set_action(GreyAction k, bool is_down) {

  bool was_down = key_down(k);

  if (is_down) {
    keys[k] |= GREY_KEY_DOWN;
  } else {
    keys[k] &= ~GREY_KEY_DOWN;
  }
  if (!was_down && is_down) {
    keys[k] |= GREY_KEY_PRESSED;
  }
  if (was_down && !is_down) {
    keys[k] |= GREY_KEY_RELEASED;
  }
}

void grey_input_begin_frame() {
  for (GreyAction k = 0; k < ACTION_MAX; ++k) {
    keys[k] &= GREY_KEY_DOWN;
  }
}

void grey_input_update() {
  bool touch_down[ACTION_MAX] = {false};
  bool kb_down[ACTION_MAX] = {false};
  bool pad_down[ACTION_MAX] = {false};

#ifdef GREY_USE_TOUCH
  i32 touch_count = GetTouchPointCount();
  for (i32 i = 0; i < touch_count; ++i) {
    Vector2 pos = GetTouchPosition(i);

    for (GreyAction j = 0; j < ACTION_MAX; ++j) {
      if (CheckCollisionPointRec(pos, button_area[j])) {
        touch_down[j] = true;
      }
    }
  }
#endif

  i32 pad_index = 0;
  if (IsGamepadAvailable(pad_index)) {
    for (GreyAction k = 0; k < ACTION_MAX; ++k) {
      if (IsGamepadButtonDown(pad_index, pad_map[k])) {
        pad_down[k] = true;
      }
    }
  }

  for (GreyAction k = 0; k < ACTION_MAX; ++k) {
    if (IsKeyDown(key_map[k])) {
      kb_down[k] = true;
    }
  }

  for (GreyAction k = 0; k < ACTION_MAX; ++k) {
    key_set_action(k, touch_down[k] || kb_down[k] || pad_down[k]);
  }
}

bool grey_input_is_down(GreyAction action) { return key_down(action); }

bool grey_input_is_pressed(GreyAction action) { return key_pressed(action); }

bool grey_input_is_released(GreyAction action) { return key_released(action); }

void grey_input_draw_gamepad() {
#ifdef GREY_USE_TOUCH
  Color pad_color = Fade(LIGHTGRAY, 0.5f);
  Color border_color = Fade(DARKGRAY, 0.8f);

  for (GreyAction k = 0; k < ACTION_MAX; ++k) {
    if (button_area[k].width > 0 && button_area[k].height > 0) {
      DrawRectangleRec(button_area[k], pad_color);
      DrawRectangleLinesEx(button_area[k], 2.0f, border_color);
    }
  }
#endif
}

