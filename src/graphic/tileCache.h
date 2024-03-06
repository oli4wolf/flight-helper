
#ifndef TILECACHE_H
#define TILECACHE_H

// Include any necessary headers here
#include "globalVariables.h"
#include "loadTiles.h"
#include "calculate/calcCoords.h"
#include "graphic/shiftTiles.h"

// Define any global variables here
const int TILE_SIZE = 256; // Downloaded Tilesizes from Swiss Topo.
const int n_sprite_x = 3;  // M5Core2 has a screen resolution of 320x240, so 3x3 tiles fit perfectly.
const int n_sprite_y = 3;
const int n_sprite = n_sprite_x * n_sprite_y;
extern sprite_struct *tile_cache[n_sprite];

// Define your class or functions here
void initCanvas();
void initTileCache();
void fillTileCache(double lat, double lon);
void reloadTileCache();

void drawTileCache(sprite_struct *tile_cache[], str_pxl_coords &idx_coords);

#endif // TILECACHE_H
