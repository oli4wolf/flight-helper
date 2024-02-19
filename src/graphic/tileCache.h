
#ifndef TILECACHE_H
#define TILECACHE_H

// Include any necessary headers here
#include "globalVariables.h"
#include "loadTiles.h"
#include "calculate/calcCoords.h"

// Define any global variables here
const int tile_size = 256; // Downloaded Tilesizes from Swiss Topo.
const int n_sprite_x = 3; // M5Core2 has a screen resolution of 320x240, so 3x3 tiles fit perfectly.
const int n_sprite_y = 3;
const int n_sprite = n_sprite_x * n_sprite_y;
extern sprite_struct *tile_cache[n_sprite];

// Define your class or functions here
void initTileCache();
void fillTileCache();

#endif // TILECACHE_H
