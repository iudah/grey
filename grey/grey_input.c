#include "grey_input.h"
#include "raylib.h"
#include "type_alias.h"
#ifdef GREY_USE_TOUCH
#include "raymath.h"
#endif
#include <stdbool.h>
#include <stdint.h>

#define DPAD_MAX (4)
#define ACT_BTN_MAX (ACTION_MAX - DPAD_MAX)

typedef u8 VirtualKey;
typedef enum {
  GREY_KEY_DOWN = 1 << 0,
  GREY_KEY_PRESSED = 1 << 1,
  GREY_KEY_RELEASED = 1 << 2
} VirtualKeyAction;
typedef enum { STICK_BASE, STICK_KNOB } GreyStick;
typedef struct {
  Vector2 btn_cntr;
  f32 btn_rad;
} VirtualButton;
VirtualKey keys[ACTION_MAX] = {0};
VirtualButton button[ACT_BTN_MAX] = {0};
Rectangle button_area[DPAD_MAX];
struct {
  Vector2 pad_center;
  f32 radius;
} knobs[2];
Vector2 stick_vec = {0};
GreyPadMode gamepad_mode;

f32 base_unit;

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

static inline f32 grey_compute_base_unit() {
  f32 base_unit = GetScreenHeight() * 0.25;
  if (base_unit < 100.0f)
    base_unit = 100.0f;
  if (base_unit > 250.0f)
    base_unit = 250.0f;

  return base_unit;
}

static inline void grey_touch_input_init(u8 v_pad_mode) {
  // Load button positions from asset/button.*

  gamepad_mode = v_pad_mode;

  f32 sh = (f32)GetScreenHeight();

  base_unit = grey_compute_base_unit();

  f32 padding = base_unit * 0.65f;
  f32 base_radius = base_unit * 0.75f;

  f32 pad_base_x = padding + base_radius;
  f32 pad_base_y = sh - padding - base_radius;

  knobs[STICK_BASE] =
      (typeof(knobs[STICK_BASE])){{pad_base_x, pad_base_y}, base_unit};
  knobs[STICK_KNOB] =
      (typeof(knobs[STICK_KNOB])){{pad_base_x, pad_base_y}, base_unit * 0.4f};

  f32 btn_size = base_unit * 0.5f;

  button_area[ACTION_LEFT] =
      (Rectangle){pad_base_x - (btn_size * 1.5f),
                  pad_base_y - (btn_size * 0.5f), btn_size, btn_size};
  button_area[ACTION_RIGHT] =
      (Rectangle){pad_base_x + (btn_size * 0.5f),
                  pad_base_y - (btn_size * 0.5f), btn_size, btn_size};
  button_area[ACTION_UP] =
      (Rectangle){pad_base_x - (btn_size * 0.5f),
                  pad_base_y - (btn_size * 1.5f), btn_size, btn_size};
  button_area[ACTION_DOWN] =
      (Rectangle){pad_base_x - (btn_size * 0.5f),
                  pad_base_y + (btn_size * 0.5f), btn_size, btn_size};
}

void grey_input_init(GreyPadMode v_pad_mode) {
#ifdef GREY_USE_TOUCH
  grey_touch_input_init(v_pad_mode);
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
  if (gamepad_mode == GREY_DPAD) {
    for (i32 i = 0; i < touch_count; ++i) {
      Vector2 pos = GetTouchPosition(i);

      for (GreyAction j = 0; j < DPAD_MAX; ++j) {
        if (CheckCollisionPointRec(pos, button_area[j])) {
          touch_down[j] = true;
        }
      }
    }
  }

  knobs[STICK_KNOB].pad_center = knobs[STICK_BASE].pad_center;
  stick_vec = (Vector2){0, 0};

  if (gamepad_mode == GREY_JOYSTICK) {
    for (i32 i = 0; i < touch_count; ++i) {
      Vector2 pos = GetTouchPosition(i);

      f32 dist = Vector2Distance(pos, knobs[STICK_BASE].pad_center);
      if (dist > base_unit * 1.5f)
        continue;
      if (dist < base_unit * 0.1f)
        continue;

      auto visual_dist = dist > base_unit ? base_unit : dist;
      stick_vec =
          Vector2Normalize(Vector2Subtract(pos, knobs[STICK_BASE].pad_center));
      knobs[STICK_KNOB].pad_center = Vector2Add(
          knobs[STICK_BASE].pad_center,
          Vector2Multiply(stick_vec, (Vector2){visual_dist, visual_dist}));

      if (stick_vec.x > 0.5f)
        touch_down[ACTION_RIGHT] = true;
      if (stick_vec.x < -0.5f)
        touch_down[ACTION_LEFT] = true;
      if (stick_vec.y > 0.5f)
        touch_down[ACTION_DOWN] = true;
      if (stick_vec.y < -0.5f)
        touch_down[ACTION_UP] = true;
    }
  }
  for (i32 i = 0; i < touch_count; ++i) {
    Vector2 pos = GetTouchPosition(i);

    for (GreyAction j = 0; j < ACT_BTN_MAX; ++j) {
      if (button[j].btn_rad > 1e-5 &&
          CheckCollisionPointCircle(pos, button[j].btn_cntr,
                                    button[j].btn_rad)) {
        touch_down[j + DPAD_MAX] = true;
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

Vector2 grey_get_joystick() { return stick_vec; }

static inline void grey_input_draw_game_stick() {
#ifdef GREY_USE_TOUCH
  Color pad_color = Fade(LIGHTGRAY, 0.25f);
  Color border_color = Fade(DARKGRAY, 0.8f);

  for (GreyStick k = 0; k < 2; ++k) {
    if (knobs[k].radius > 0) {
      DrawCircleV(knobs[k].pad_center, knobs[k].radius, pad_color);
      DrawCircleLinesV(knobs[k].pad_center, knobs[k].radius, border_color);
    }
  }
#endif
}

static inline void grey_input_draw_game_dpad() {
#ifdef GREY_USE_TOUCH
  Color pad_color = Fade(LIGHTGRAY, 0.5f);
  Color border_color = Fade(DARKGRAY, 0.8f);

  for (GreyAction k = 0; k < DPAD_MAX; ++k) {
    if (button_area[k].width > 0 && button_area[k].height > 0) {
      DrawRectangleRec(button_area[k], pad_color);
      DrawRectangleLinesEx(button_area[k], 2.0f, border_color);
    }
  }
#endif
}

static inline void grey_input_draw_game_action_pad() {

  Color pad_color = Fade(LIGHTGRAY, 0.5f);
  Color border_color = Fade(DARKGRAY, 0.8f);

  for (GreyAction k = 0; k < ACT_BTN_MAX; ++k) {
    if (button[k].btn_rad > 0) {
      DrawCircleV(button[k].btn_cntr, button[k].btn_rad, pad_color);
      DrawCircleLinesV(button[k].btn_cntr, button[k].btn_rad, border_color);
    }
  }
}

void grey_input_draw_gamepad() {
  if (gamepad_mode == GREY_DPAD)
    grey_input_draw_game_dpad();
  if (gamepad_mode == GREY_JOYSTICK)
    grey_input_draw_game_stick();
  grey_input_draw_game_action_pad();
}

void grey_set_action_btn(GreyAction action, Vector2 cntr, f32 rad) {
  if (action < DPAD_MAX || action >= ACTION_MAX)
    return;
  button[action - DPAD_MAX].btn_cntr = cntr;
  button[action - DPAD_MAX].btn_rad = rad;
}

void grey_default_action_btn() {
  f32 sw = (f32)GetScreenWidth();
  f32 sh = (f32)GetScreenHeight();

  f32 padding = base_unit * 0.65f;
  f32 base_radius = base_unit * 0.75f;

  f32 act_base_x = sw - padding - base_radius;
  f32 act_base_y = sh - padding - base_radius;

  f32 btn_rad = base_unit * 0.35f;
  f32 offset = base_unit * 0.85f;

  grey_set_action_btn(ACTION_A, (Vector2){act_base_x + offset, act_base_y},
                      btn_rad);

  grey_set_action_btn(ACTION_X, (Vector2){act_base_x, act_base_y + offset},
                      btn_rad);

  grey_set_action_btn(ACTION_O, (Vector2){act_base_x, act_base_y - offset},
                      btn_rad);

  grey_set_action_btn(ACTION_D, (Vector2){act_base_x - offset, act_base_y},
                      btn_rad);
}
