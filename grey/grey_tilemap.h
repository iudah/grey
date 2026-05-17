#ifndef GREY_TILEMAP_H
#define GREY_TILEMAP_H

#include "raylib.h"
#include "type_alias.h"

typedef struct {
  u8 *grid;
  u32 grid_height;
  u32 grid_width;
  u32 tile_size;
} GreyTileMap;

typedef enum { EMPTY_TILE, SOLID_TILE, TRIGGER_TILE } TileTriggerType;

typedef struct {
  Rectangle tex_src;
  bool solid;
  bool trigger;
} TileDef;

bool grey_draw_tilemap(GreyTileMap *map);

#endif
