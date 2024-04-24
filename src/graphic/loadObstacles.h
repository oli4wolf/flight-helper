#ifndef LOADOBSTACLES_H
#define LOADOBSTACLES_H

// Include any necessary libraries or headers
#include "SD.h"
#include "globalVariables.h"
#include "drawing.h"

// Declare your functions or classes here
void loadObstaclesLines(LGFX_Sprite *sprite, int zoom, int tile_x, int tile_y);
void loadObstaclesPoints(LGFX_Sprite *sprite, int zoom, int tile_x, int tile_y);

#endif // LOADOBSTACLES_H