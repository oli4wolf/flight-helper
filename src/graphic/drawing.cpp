#include "drawing.h"

// Implement the functions defined in drawing.h here
void drawLineThickness(LGFX_Sprite *sprite, int x1, int x2, int y1, int y2, int color)
{
    // Bruteforce thickness.
    // Each permutation around the end and starting point is drawn. +1, -1.
    sprite->drawLine(x1 - 1, y1 - 1, x2 - 1, y2 - 1, color); //-1,-1
    sprite->drawLine(x1, y1 - 1, x2, y2 - 1, color);         // 0,-1
    sprite->drawLine(x1 + 1, y1 - 1, x2 + 1, y2 - 1, color); // 1,-1

    sprite->drawLine(x1 - 1, y1, x2 - 1, y2, color); //-1,0
    // sprite->drawLine(x1, y1, x2, y2, TFT_MAGENTA); //0,0
    sprite->drawLine(x1 + 1, y1, x2 + 1, y2, color); // 1,0

    sprite->drawLine(x1 - 1, y1 + 1, x2 - 1, y2 + 1, color); //-1,1
    sprite->drawLine(x1, y1 + 1, x2, y2 + 1, color);         // 0,1
    sprite->drawLine(x1 + 1, y1 + 1, x2 + 1, y2 + 1, color); // 1,1
}
