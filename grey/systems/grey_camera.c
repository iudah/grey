#include "grey_ecs.h"
#include "raylib.h"
#include "raymath.h"

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

  camera->target.x +=
      (cam_data->target.x - camera->target.x) * cam_data->speed * delta_time;
  camera->target.y +=
      (cam_data->target.y - camera->target.y) * cam_data->speed * delta_time;
}
