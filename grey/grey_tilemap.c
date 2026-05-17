#include "grey_tilemap.h"
#include "grey_assert.h"
#include "raylib.h"
#include "type_alias.h"

bool grey_draw_tilemap(GreyTileMap *map) {
  GREY_ASSERT(map, "Null map");
  if (!map || !map->grid)
    return false;

  for (u32 grid_y = 0; grid_y < map->grid_height; ++grid_y) {
    for (u32 grid_x = 0; grid_x < map->grid_width; ++grid_x) {
      auto tile_id = map->grid[(grid_y * map->grid_width) + grid_x];

      // Let 0=empty/air tile
      if (tile_id == 0)
        continue;

      f32 draw_x = grid_x * map->tile_size;
      f32 draw_y = grid_y * map->tile_size;

      // Error tile
      Color tile_texture = RED;
      switch (tile_id) {
      case 1:
        tile_texture = DARKGREEN;
        break;
      case 2:
        tile_texture = DARKGRAY;
        break;
      case 3:
        tile_texture = BLUE;
        break;
      }

      DrawRectangle(draw_x, draw_y, map->tile_size, map->tile_size,
                    tile_texture);

#define ALPHA (0.15f)
      // Draw grid
      DrawRectangleLines(draw_x, draw_y, map->tile_size, map->tile_size,
                         Fade(BLACK, ALPHA));
    }
  }
  return true;
}
