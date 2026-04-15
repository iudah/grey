#include "grey_ecs.h"
#include "raylib.h"

#define SYS_RENDER_MASK (COMP_POSITION|COMP_RENDER)

void grey_sys_render_draw(EcsRegistry reg){
  u32 n_entities = ecs_get_number_of_entities(reg);
  for(u32 i=0; i<n_entities; ){
    Entity e = ++i;
    if((ecs_get_mask(reg, e)&SYS_RENDER_MASK)==SYS_RENDER_MASK){
      PositionComponent *pos = ecs_get_position(reg,e);
      RenderComponent *ren = ecs_get_render(reg, e);

      DrawRectangle(pos->x, pos->y, ren->width, ren->height, ren->color);
    }
  }
}

