#include "graphic/shiftTiles.h"
#include "shiftTiles.h"

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

Shift left.
            [0,1,2]     [1,2,-]
            [3,4,5]     [4,5,-]
            [6,7,8] to  [7,8,-]
*/

shift_direction determineShiftTileDirection(int center_tile_x, int center_tile_y, sprite_struct *tile_cache[], int n_sprite_x,
                                            int n_sprite_y)
{

    if (center_tile_x == tile_cache[4]->tile_x + 1)
    {
        return LEFT;
    }
    else if (center_tile_x == tile_cache[4]->tile_x - 1)
    {
        return RIGHT;
    }
    else if (center_tile_y == tile_cache[4]->tile_y + 1)
    {
        return UP;
    }
    else if (center_tile_y == tile_cache[4]->tile_y - 1)
    {
        return DOWN;
    }
    else
    {
        return NONE;
    }
}

shift_direction shiftTileDirection(int center_tile_x, int center_tile_y, sprite_struct *tile_cache[], int n_sprite_x,
                                   int n_sprite_y)
{

    if (center_tile_x == tile_cache[4]->tile_x + 1)
    {
        shiftTilesLeft(tile_cache, n_sprite_x, n_sprite_y);
        return LEFT;
    }
    else if (center_tile_x == tile_cache[4]->tile_x - 1)
    {
        shiftTilesRight(tile_cache, n_sprite_x, n_sprite_y);
        return RIGHT;
    }
    else if (center_tile_y == tile_cache[4]->tile_y + 1)
    {
        shiftTilesUp(tile_cache, n_sprite_x, n_sprite_y);
        return UP;
    }
    else if (center_tile_y == tile_cache[4]->tile_y - 1)
    {
        shiftTilesDown(tile_cache, n_sprite_x, n_sprite_y);
        return DOWN;
    }
    else
    {
        return NONE;
    }
}

void invalidateTile(sprite_struct *ptr_tmp)
{
    ptr_tmp->zoom = -1;
    ptr_tmp->tile_x = -1;
    ptr_tmp->tile_y = -1;
    ptr_tmp->sprite->fillSprite(TFT_BROWN);
}

// Use a reload Cache after this function to fill the invalidatet tiles.
void shiftTilesLeft(sprite_struct *tile_cache[], int n_sprite_x,
                    int n_sprite_y)
{
    sprite_struct *ptr_tmp;

    for (int i_y = 0; i_y < n_sprite_y; i_y++)
    {
        ptr_tmp = tile_cache[n_sprite_x * i_y];

        for (int i_x = 0; i_x < n_sprite_x - 1; i_x++)
        {
            tile_cache[n_sprite_x * i_y + i_x] =
                tile_cache[n_sprite_x * i_y + i_x + 1];
        }

        tile_cache[n_sprite_x * i_y + n_sprite_x - 1] = ptr_tmp;
        invalidateTile(ptr_tmp);
    }
}

void shiftTilesRight(sprite_struct *tile_cache[], int n_sprite_x,
                     int n_sprite_y)
{
    sprite_struct *ptr_tmp;

    for (int i_y = 0; i_y < n_sprite_y; i_y++)
    {
        ptr_tmp = tile_cache[n_sprite_x * i_y + n_sprite_x - 1];

        for (int i_x = n_sprite_x - 1; i_x > 0; i_x--)
        {
            tile_cache[n_sprite_x * i_y + i_x] =
                tile_cache[n_sprite_x * i_y + i_x - 1];
        }

        tile_cache[n_sprite_x * i_y] = ptr_tmp;
        invalidateTile(ptr_tmp);
    }
}

void shiftTilesUp(sprite_struct *tile_cache[], int n_sprite_x,
                  int n_sprite_y)
{
    sprite_struct *ptr_tmp;

    for (int i_x = 0; i_x < n_sprite_x; i_x++)
    {
        ptr_tmp = tile_cache[i_x];

        for (int i_y = 0; i_y < n_sprite_y - 1; i_y++)
        {
            tile_cache[n_sprite_x * i_y + i_x] =
                tile_cache[n_sprite_x * (i_y + 1) + i_x];
        }

        tile_cache[n_sprite_x * (n_sprite_y - 1) + i_x] = ptr_tmp;
        invalidateTile(ptr_tmp);
    }
}

void shiftTilesDown(sprite_struct *tile_cache[], int n_sprite_x,
                    int n_sprite_y)
{
    // Shift the tiles to the left
    sprite_struct *ptr_tmp;

    for (int i_x = 0; i_x < n_sprite_x; i_x++)
    {
        ptr_tmp = tile_cache[n_sprite_x * (n_sprite_y - 1) + i_x];

        for (int i_y = n_sprite_y - 1; i_y > 0; i_y--)
        {
            tile_cache[n_sprite_x * i_y + i_x] =
                tile_cache[n_sprite_x * (i_y - 1) + i_x];
        }

        tile_cache[i_x] = ptr_tmp;
        invalidateTile(ptr_tmp);
    }
}