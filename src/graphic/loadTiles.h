#ifndef LOADTILES_H
#define LOADTILES_H

// Include any necessary libraries or headers here
#include "SD.h"
#include "globalVariables.h"
#include "drawing.h"

extern SemaphoreHandle_t semDrawScreen;

// Declare your class or function prototypes here
void loadTile(LGFX_Sprite *sprite, int zoom, int tile_x, int tile_y);

#endif // LOADTILES_H