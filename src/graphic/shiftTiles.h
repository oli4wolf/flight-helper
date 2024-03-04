#ifndef SHIFTTILES_H
#define SHIFTTILES_H

// Include any necessary headers here
#include "globalVariables.h"

// Declare your functions or classes here
void shiftTilesLeft(sprite_struct *tile_cache[], int n_sprite_x,
                        int n_sprite_y);
void shiftTilesRight(sprite_struct *tile_cache[], int n_sprite_x,
                        int n_sprite_y);
void shiftTilesUp(sprite_struct *tile_cache[], int n_sprite_x,
                        int n_sprite_y);
void shiftTilesDown(sprite_struct *tile_cache[], int n_sprite_x,
                        int n_sprite_y);

#endif // SHIFTTILES_H
