#include "tileCache.h"

sprite_struct *tile_cache[n_sprite];
int zoom = 15;
str_pxl_coords curr_gps_pxl_coords = {0, 0};
str_pxl_coords display_center_pxl_coords = {0, 0};

// Implement your functions here
void initTileCache()
{
    ESP_LOGD("initTileCache", "heap_caps_get_free_size(MALLOC_CAP_DMA):   %8d",
             heap_caps_get_free_size(MALLOC_CAP_DMA));
    ESP_LOGD("initTileCache", "heap_caps_get_free_size(MALLOC_CAP_SPIRAM):%8d",
             heap_caps_get_free_size(MALLOC_CAP_SPIRAM));

    for (int i = 0; i < n_sprite; i++)
    {
        tile_cache[i] = (sprite_struct *)ps_malloc(sizeof(sprite_struct));

        tile_cache[i]->zoom = 0;
        tile_cache[i]->tile_x = 0;
        tile_cache[i]->tile_y = 0;

        tile_cache[i]->sprite = new LGFX_Sprite(&canvas);
        tile_cache[i]->sprite->setPsram(true);
        tile_cache[i]->sprite->createSprite(tile_size, tile_size);
        tile_cache[i]->sprite->fillSprite(TFT_GREEN);
    }

    ESP_LOGI("initTileCache", "tile_cache was allocated.");
    ESP_LOGD("initTileCache", "heap_caps_get_free_size(MALLOC_CAP_DMA):   %8d",
             heap_caps_get_free_size(MALLOC_CAP_DMA));
    ESP_LOGD("initTileCache", "heap_caps_get_free_size(MALLOC_CAP_SPIRAM):%8d",
             heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
}

void fillTileCache()
{
    zoom = 15;
    // Start with a fixed coordinates 	46.95234, 7.45282
    calcCoords2CoordsIdx(curr_gps_pxl_coords, 46.95234, 7.45282, zoom, tile_size);

    // Reset to default Zoom.

    str_tile_coords tile_coords;
    calcCoordsIdx2Tile(tile_coords, curr_gps_pxl_coords, tile_size);

    int center_tile_x = tile_coords.tile_x;
    int center_tile_y = tile_coords.tile_y;

    for (int i_x = 0; i_x < n_sprite_x; i_x++)
    {
        for (int i_y = 0; i_y < n_sprite_y; i_y++)
        {
            int i = n_sprite_x * i_y + i_x;
            int tile_x = center_tile_x + i_x - n_sprite_x / 2;
            int tile_y = center_tile_y + i_y - n_sprite_y / 2;
#ifdef __printTileCache__
            ESP_LOGD("updateTileCache", "    tile (%d,%d,%d[%d],%d[%d]) -> ", i, zoom, tile_cache[i]->tile_x, tile_x, tile_cache[i]->tile_y, tile_y);
#endif
            if (!(tile_cache[i]->zoom == zoom && tile_cache[i]->tile_x == tile_x &&
                  tile_cache[i]->tile_y == tile_y))
            {
                ESP_LOGI("updateTileCache", "changed, update required. ");
                // bug while not initialised it is allways 0 0 
                loadTile(tile_cache[i]->sprite, zoom, tile_x, tile_y);
            }
        }
    }
}
