#include "graphic/shiftTiles.h"

sprite_struct *ptr_tmp;

/**
 *  
    for (int i_x = 0; i_x < n_sprite_x; i_x++)
    {
        for (int i_y = 0; i_y < n_sprite_y; i_y++)
        {
            int i = n_sprite_x (3) * i_y + i_x;
            int tile_x = center_tile_x + i_x - n_sprite_x(3) / 2; (3/2 = 1.5 -> 1)
            int tile_y = center_tile_y + i_y - n_sprite_y / 2;

            [0,1,2]
            [3,4,5]
            [6,7,8]
*/

/*
Shift left.
            [0,1,2]
            [3,4,5]
            [6,7,8] to 

            [1,2,-]
            [4,5,-]
            [7,8,-]
*/

// Use a reload Cache after this function to fill the invalidatet tiles.
void shiftTilesLeft(sprite_struct *tile_cache[], int n_sprite_x,
                    int n_sprite_y)
{
    // Shift the tiles to the left
    for (int i_y = 0; i_y < n_sprite_y; i_y++)
    {
        for (int i_x = 0; i_x < n_sprite_x-1; i_x++) // Minus 1 to avoid out of bounds.
        {
            tile_cache[n_sprite_x * i_y + i_x] = tile_cache[n_sprite_x * i_y + i_x + 1];
        }
    }

    // Invalidate the right Tiles.
    for (int i_y = 0; i_y < n_sprite_y; i_y++)
    {
        tile_cache[n_sprite_x * i_y + 2]->tile_x = -1;
        tile_cache[n_sprite_x * i_y + 2]->tile_y = -1;
        tile_cache[n_sprite_x * i_y + 2]->sprite->fillSprite(TFT_BROWN);
    }
}

void shiftTilesRight(sprite_struct *tile_cache[], int n_sprite_x,
                     int n_sprite_y)
{
    // Shift the tiles to the left
    for (int i_y = 0; i_y < n_sprite_y; i_y++)
    {
        for (int i_x = 1; i_x < n_sprite_x; i_x++) // Minus 1 to avoid out of bounds.
        {
            tile_cache[n_sprite_x * i_y + i_x] = tile_cache[n_sprite_x * i_y + i_x - 1];
        }
    }

    // Invalidate the right Tiles.
    for (int i_y = 0; i_y < n_sprite_y; i_y++)
    {
        tile_cache[n_sprite_x * i_y]->tile_x = -1;
        tile_cache[n_sprite_x * i_y]->tile_y = -1;
        tile_cache[n_sprite_x * i_y]->sprite->fillSprite(TFT_BROWN);
    }
}

void shiftTilesUp(sprite_struct *tile_cache[], int n_sprite_x,
                  int n_sprite_y)
{
    // Shift the tiles to the left
    for (int i_x = 0; i_x < n_sprite_x; i_x++)
    {
        for (int i_y = 0; i_y < n_sprite_y-1; i_y++) // Minus 1 to avoid out of bounds.
        {
            tile_cache[n_sprite_x * i_y + i_x] = tile_cache[n_sprite_x * i_y + i_x + n_sprite_x];
        }
    }

    // Invalidate the right Tiles.
    for (int i_x = 0; i_x < n_sprite_x; i_x++)
    {
        tile_cache[((n_sprite_y-1) * n_sprite_x) + i_x]->tile_x = -1;
        tile_cache[((n_sprite_y-1) * n_sprite_x) + i_x]->tile_y = -1;
        tile_cache[((n_sprite_y-1) * n_sprite_x) + i_x]->sprite->fillSprite(TFT_BROWN);
    }
}

void shiftTilesDown(sprite_struct *tile_cache[], int n_sprite_x,
                    int n_sprite_y)
{
    // Shift the tiles to the left
    for (int i_x = 0; i_x < n_sprite_x; i_x++)
    {
        for (int i_y = 1; i_y < n_sprite_y; i_y++) // Minus 1 to avoid out of bounds.
        {
            tile_cache[n_sprite_x * i_y + i_x] = tile_cache[n_sprite_x * i_y + i_x + n_sprite_x];
        }
    }

    // Invalidate the right Tiles.
    for (int i_x = 0; i_x < n_sprite_x; i_x++)
    {
        tile_cache[i_x]->tile_x = -1;
        tile_cache[i_x]->tile_y = -1;
        tile_cache[i_x]->sprite->fillSprite(TFT_BROWN);
    }
}