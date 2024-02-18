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

#endif // __COORDSTRUCT_H__
