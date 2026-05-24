#include "grey_ecs.h"
#include "grey_rand.h"
#include "raylib.h"
#include "raymath.h"

#define DECAY_ALPHA 0.7f
#define ___CLAMP___(x, lo, hi) (x < lo ? lo : (x > hi ? hi : x))
#define CLAMP(x, lo, hi) ___CLAMP___((x), (lo), (hi))

void grey_sys_camera_update(EcsRegistry reg, Entity camera_ntt,
                            Camera2D *camera) {
  auto ntt_mask = ecs_get_mask(reg, camera_ntt);
  if ((ntt_mask & COMP_CAMERA) != COMP_CAMERA)
    return;

  auto delta_time = GetFrameTime();

  CameraComponent *cam_data = ecs_get_camera(reg, camera_ntt);

  auto target_pos = ecs_get_position(reg, cam_data->target_entity);
  if (!target_pos)
    return;

  auto deficit = Vector2Subtract(*(Vector2 *)target_pos, cam_data->target);
  if (deficit.x > cam_data->half_bound.x)
    cam_data->target.x = target_pos->x - cam_data->half_bound.x;
  if (deficit.x < -cam_data->half_bound.x)
    cam_data->target.x = target_pos->x + cam_data->half_bound.x;
  if (deficit.y > cam_data->half_bound.y)
    cam_data->target.y = target_pos->y - cam_data->half_bound.y;
  if (deficit.y < -cam_data->half_bound.y)
    cam_data->target.y = target_pos->y + cam_data->half_bound.y;

  Vector2 trauma = {0, 0};
  if (cam_data->shake_trauma > 0) {
    auto shake = cam_data->shake_trauma;
    trauma =
        (Vector2){grey_rand_f32(-1, 1) * shake, grey_rand_f32(-1, 1) * shake};
    cam_data->shake_trauma *= DECAY_ALPHA;
  }

  camera->target.x +=
      (cam_data->target.x - camera->target.x) * cam_data->speed * delta_time;
  camera->target.y +=
      (cam_data->target.y - camera->target.y) * cam_data->speed * delta_time;

  camera->target.x =
      Clamp(camera->target.x + trauma.x, cam_data->min.x, cam_data->max.x);
  camera->target.y =
      Clamp(camera->target.y + trauma.y, cam_data->min.y, cam_data->max.y);
}

void grey_set_camera_trauma(EcsRegistry reg, Entity camera, f32 trauma_pxs) {
  auto cam = ecs_get_camera(reg, camera);
  cam->shake_trauma = trauma_pxs;
}
