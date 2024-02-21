#ifndef COORDSTRUCT_H
#define COORDSTRUCT_H

#include <M5Unified.h>

struct sprite_struct
{
  int zoom;
  int tile_x;
  int tile_y;
  LGFX_Sprite *sprite;
};

struct center_point
{
  int zoom;
  int tile_x;
  int tile_y;
  int x;
  int y;
};

// Struct Part
struct str_tile_coords
{
    int zoom;
    int tile_x;
    int tile_y;
};
struct str_pxl_on_tile
{
    int pxl_x;
    int pxl_y;
};
struct str_pxl_coords
{
    int pxl_x;
    int pxl_y;
};

#endif // __COORDSTRUCT_H__
