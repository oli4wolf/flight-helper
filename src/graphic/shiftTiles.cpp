#include "graphic/shiftTiles.h"

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
sprite_struct *tmp_ptr[3];

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

// Use a reload Cache after this function to fill the invalidatet tiles.
void shiftTilesLeft(sprite_struct *tile_cache[], int n_sprite_x,
                    int n_sprite_y)
{
    for (int i = 0; i < n_sprite_x * n_sprite_y; i++)
    {
        ESP_LOGD("ShiftTileCacheLeft", "Before tile (i=%d,zoom=%d,tile_x=%d,tile_y=%d)", i, zoom, tile_cache[i]->tile_x, tile_cache[i]->tile_y);
    }

    // Keep the Pointer to be placed Right.
    for (int i_y = 0; i_y < n_sprite_y; i_y++)
    {
        tmp_ptr[i_y] = tile_cache[n_sprite_x * 3 + 0];
    }

    // Shift the tiles to the left
    for (int i_y = 0; i_y < n_sprite_y; i_y++)
    {
        for (int i_x = 0; i_x < n_sprite_x - 1; i_x++) // Minus 1 to avoid out of bounds.
        {
            tile_cache[n_sprite_x * i_y + i_x] = tile_cache[n_sprite_x * i_y + i_x + 1];
        }
    }

    for (int i = 0; i < n_sprite_x * n_sprite_y; i++)
    {
        ESP_LOGD("ShiftTileCacheLeft", "Middle tile (i=%d,zoom=%d,tile_x=%d,tile_y=%d)", i, zoom, tile_cache[i]->tile_x, tile_cache[i]->tile_y);
    }

    // Invalidate the right Tiles.
    for (int i_y = 0; i_y < n_sprite_y; i_y++)
    {
        // Use the tmp_ptrs to fill the right side.
        tile_cache[n_sprite_x * i_y + 2] = tmp_ptr[i_y];
        tile_cache[n_sprite_x * i_y + 2]->tile_x = -1;
        tile_cache[n_sprite_x * i_y + 2]->tile_y = -1;
        tile_cache[n_sprite_x * i_y + 2]->zoom = -1;
        tile_cache[n_sprite_x * i_y + 2]->sprite->fillSprite(TFT_BROWN);
    }

    for (int i = 0; i < n_sprite_x * n_sprite_y; i++)
    {
        ESP_LOGD("ShiftTileCacheLeft", "Inbetween tile (i=%d,zoom=%d,tile_x=%d,tile_y=%d)", i, zoom, tile_cache[i]->tile_x, tile_cache[i]->tile_y);
    }
}

void shiftTilesRight(sprite_struct *tile_cache[], int n_sprite_x,
                     int n_sprite_y)
{
    for (int i = 0; i < n_sprite_x * n_sprite_y; i++)
    {
        ESP_LOGD("ShiftTileCacheRight", "Before tile (i=%d,zoom=%d,tile_x=%d,tile_y=%d)", i, zoom, tile_cache[i]->tile_x, tile_cache[i]->tile_y);
    }

    // Keep the Pointer to be placed Right.
    for (int i_y = 0; i_y < n_sprite_y; i_y++)
    {
        tmp_ptr[i_y] = tile_cache[n_sprite_x * 3 + 2];
    }

    // Shift the tiles to the left
    for (int i_y = 0; i_y < n_sprite_y; i_y++)
    {
        for (int i_x = 1; i_x < n_sprite_x; i_x++) // Minus 1 to avoid out of bounds.
        {
            tile_cache[n_sprite_x * i_y + i_x] = tile_cache[n_sprite_x * i_y + i_x - 1];
        }
    }

    for (int i = 0; i < n_sprite_x * n_sprite_y; i++)
    {
        ESP_LOGD("ShiftTileCacheRight", "Middle tile (i=%d,zoom=%d,tile_x=%d,tile_y=%d)", i, zoom, tile_cache[i]->tile_x, tile_cache[i]->tile_y);
    }

    // Invalidate the left Tiles.
    for (int i_y = 0; i_y < n_sprite_y; i_y++)
    {
        // Use the tmp_ptrs to fill the left side.
        tile_cache[n_sprite_x * i_y] = tmp_ptr[i_y];
        tile_cache[n_sprite_x * i_y]->tile_x = -1;
        tile_cache[n_sprite_x * i_y]->tile_y = -1;
        tile_cache[n_sprite_x * i_y]->zoom = -1;
        tile_cache[n_sprite_x * i_y]->sprite->fillSprite(TFT_BROWN);
    }

    for (int i = 0; i < n_sprite_x * n_sprite_y; i++)
    {
        ESP_LOGD("ShiftTileCacheRight", "After tile (i=%d,zoom=%d,tile_x=%d,tile_y=%d)", i, zoom, tile_cache[i]->tile_x, tile_cache[i]->tile_y);
    }
}

void shiftTilesUp(sprite_struct *tile_cache[], int n_sprite_x,
                  int n_sprite_y)
{
    for (int i = 0; i < n_sprite_x * n_sprite_y; i++)
    {
        ESP_LOGD("ShiftTileCacheUp", "Before tile (i=%d,zoom=%d,tile_x=%d,tile_y=%d)", i, zoom, tile_cache[i]->tile_x, tile_cache[i]->tile_y);
    }

    // Shift the tiles to the left
    for (int i_x = 0; i_x < n_sprite_x; i_x++)
    {
        for (int i_y = 0; i_y < n_sprite_y - 1; i_y++) // Minus 1 to avoid out of bounds.
        {
            tile_cache[n_sprite_x * i_y + i_x] = tile_cache[n_sprite_x * i_y + i_x + n_sprite_x];
        }
    }

    for (int i = 0; i < n_sprite_x * n_sprite_y; i++)
    {
        ESP_LOGD("ShiftTileCacheUp", "Middle tile (i=%d,zoom=%d,tile_x=%d,tile_y=%d)", i, zoom, tile_cache[i]->tile_x, tile_cache[i]->tile_y);
    }

    // Invalidate the right Tiles.
    for (int i_x = 0; i_x < n_sprite_x; i_x++)
    {
        tile_cache[((n_sprite_y - 1) * n_sprite_x) + i_x]->tile_x = -1;
        tile_cache[((n_sprite_y - 1) * n_sprite_x) + i_x]->tile_y = -1;
        tile_cache[((n_sprite_y - 1) * n_sprite_x) + i_x]->sprite->fillSprite(TFT_BROWN);
    }

    for (int i = 0; i < n_sprite_x * n_sprite_y; i++)
    {
        ESP_LOGD("ShiftTileCacheUp", "After tile (i=%d,zoom=%d,tile_x=%d,tile_y=%d)", i, zoom, tile_cache[i]->tile_x, tile_cache[i]->tile_y);
    }
}

void shiftTilesDown(sprite_struct *tile_cache[], int n_sprite_x,
                    int n_sprite_y)
{
    for (int i = 0; i < n_sprite_x * n_sprite_y; i++)
    {
        ESP_LOGD("ShiftTileCacheDown", "Before tile (i=%d,zoom=%d,tile_x=%d,tile_y=%d)", i, zoom, tile_cache[i]->tile_x, tile_cache[i]->tile_y);
    }

    ESP_LOGD("shiftTilesDown", "n_sprite_x: %d, n_sprite_y: %d", n_sprite_x, n_sprite_y);
    // Shift the tiles to the left
    for (int i_x = 0; i_x < n_sprite_x; i_x++)
    {
        for (int i_y = 1; i_y < n_sprite_y; i_y++)
        {
            tile_cache[n_sprite_x * i_y + i_x] = tile_cache[(n_sprite_x * i_y) + i_x - n_sprite_x];
            ESP_LOGD("shiftTilesDown", "From: %d, to: %d.", (n_sprite_x * i_y + i_x), (n_sprite_x * i_y + i_x - n_sprite_x));
        }
    }

    for (int i = 0; i < n_sprite_x * n_sprite_y; i++)
    {
        ESP_LOGD("ShiftTileCacheDown", "Middle tile (i=%d,zoom=%d,tile_x=%d,tile_y=%d)", i, zoom, tile_cache[i]->tile_x, tile_cache[i]->tile_y);
    }

    // Invalidate the right Tiles.
    for (int i_x = 0; i_x < n_sprite_x; i_x++)
    {
        tile_cache[i_x]->tile_x = -1;
        tile_cache[i_x]->tile_y = -1;
        tile_cache[i_x]->sprite->fillSprite(TFT_BROWN);
    }

    for (int i = 0; i < n_sprite_x * n_sprite_y; i++)
    {
        ESP_LOGD("ShiftTileCacheDown", "After tile (i=%d,zoom=%d,tile_x=%d,tile_y=%d)", i, zoom, tile_cache[i]->tile_x, tile_cache[i]->tile_y);
    }
}