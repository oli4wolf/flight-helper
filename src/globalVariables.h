#include <stdio.h>
#include <M5Unified.h>
#include "coordStruct.h"

extern SemaphoreHandle_t semDrawScreen; // Initialised in main to avoid conflict SD and Display.

extern M5GFX lcd;

const int tile_size = 256; // Downloaded Tilesizes from Swiss Topo.
const int n_sprite_x = 3; // M5Core2 has a screen resolution of 320x240, so 3x3 tiles fit perfectly.
const int n_sprite_y = 3;
const int n_sprite = n_sprite_x * n_sprite_y;
extern sprite_struct *tile_cache[n_sprite];
