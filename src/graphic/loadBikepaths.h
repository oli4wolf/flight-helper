#ifndef LOADBIKEPATHS_H
#define LOADBIKEPATHS_H

// Include any necessary libraries or headers here
#include "SD.h"
#include "globalVariables.h"
#include "drawing.h"

//extern SemaphoreHandle_t semDrawScreen;

// Declare your class or function prototypes here
void loadBikepaths(LGFX_Sprite *sprite, int zoom, int tile_x, int tile_y);

#endif // LOADBIKEPATHS_H