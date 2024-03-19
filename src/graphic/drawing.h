#ifndef DRAWING_H
#define DRAWING_H

// Include any necessary libraries here
#include "globalVariables.h"

extern M5Canvas triangle_icon;

// Declare any global variables or constants here

// Declare any function prototypes here

void drawLineThickness(LGFX_Sprite *sprite, int x1, int x2, int y1, int y2, int color);
void drawGPSInfo();
void drawGPSInfoLoop(void *pvParameters);

void initDirectionIcon();
void pushDirIcon();

void drawVarioInfo();

#endif // DRAWING_H
