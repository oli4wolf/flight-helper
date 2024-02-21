#ifndef DRAWING_H
#define DRAWING_H

// Include any necessary libraries here
#include "globalVariables.h"

// Declare any global variables or constants here
extern SemaphoreHandle_t semDrawScreen;

// Declare any function prototypes here

void drawLineThickness(LGFX_Sprite *sprite, int x1, int x2, int y1, int y2, int color);

#endif // DRAWING_H