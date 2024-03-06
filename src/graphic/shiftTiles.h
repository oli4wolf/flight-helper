#ifndef SHIFTTILES_H
#define SHIFTTILES_H

// Include any necessary headers here
#include "globalVariables.h"

// Declare your functions or classes here
enum shift_direction
{
    LEFT,  // 0
    RIGHT, // 1
    UP,    // 2
    DOWN,  // 3
    NONE   // 4
};

shift_direction determineShiftTileDirection(int center_tile_x, int center_tile_y, sprite_struct *tile_cache[], int n_sprite_x,
                                   int n_sprite_y);

shift_direction shiftTileDirection(int center_tile_x, int center_tile_y, sprite_struct *tile_cache[], int n_sprite_x,
                                   int n_sprite_y);

void shiftTilesLeft(sprite_struct *tile_cache[], int n_sprite_x,
                    int n_sprite_y);
void shiftTilesRight(sprite_struct *tile_cache[], int n_sprite_x,
                     int n_sprite_y);
void shiftTilesUp(sprite_struct *tile_cache[], int n_sprite_x,
                  int n_sprite_y);
void shiftTilesDown(sprite_struct *tile_cache[], int n_sprite_x,
                    int n_sprite_y);

#endif // SHIFTTILES_H
