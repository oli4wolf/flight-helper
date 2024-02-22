#include "tileCache.h"

LGFX_Sprite canvas(&lcd); // screen buffer // Global

sprite_struct *tile_cache[n_sprite];
int zoom = 15;
str_pxl_coords curr_gps_pxl_coords = {0, 0};
str_pxl_coords display_center_pxl_coords = {0, 0};

void initCanvas()
{
    lcd.startWrite();
    canvas.setPsram(true);
    canvas.createSprite(lcd.width(), lcd.height());
    canvas.fillSprite(TFT_LIGHTGREY);
    lcd.endWrite();
}

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
        tile_cache[i]->sprite->createSprite(TILE_SIZE, TILE_SIZE);
        tile_cache[i]->sprite->fillSprite(TFT_GREEN);
    }

    ESP_LOGI("initTileCache", "tile_cache was allocated.");
    ESP_LOGD("initTileCache", "heap_caps_get_free_size(MALLOC_CAP_DMA):   %8d",
             heap_caps_get_free_size(MALLOC_CAP_DMA));
    ESP_LOGD("initTileCache", "heap_caps_get_free_size(MALLOC_CAP_SPIRAM):%8d",
             heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
}

void fillTileCache(double lat, double lon)
{
    ESP_LOGD("curr_coords", "before calc: x:%d, y: %d", curr_gps_pxl_coords.pxl_x, curr_gps_pxl_coords.pxl_y);
    // Start with a fixed coordinates 	46.95234, 7.45282
    calcCoordsToCoordsPxl(curr_gps_pxl_coords, lat, lon, zoom, TILE_SIZE);

    // Reset to default Zoom.
    reloadTileCache();
}

void reloadTileCache()
{
    str_tile_coords tile_coords;
    calcCoordsPxlToTile(tile_coords, curr_gps_pxl_coords, TILE_SIZE);
    ESP_LOGD("curr_coords", "after calc: x:%d, y: %d", curr_gps_pxl_coords.pxl_x, curr_gps_pxl_coords.pxl_y);

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
                tile_cache[i]->zoom = zoom;
                tile_cache[i]->tile_x = tile_x;
                tile_cache[i]->tile_y = tile_y;
                loadTile(tile_cache[i]->sprite, zoom, tile_x, tile_y);
                loadObstaclesPoints(tile_cache[i]->sprite, zoom, tile_x, tile_y);
                loadObstaclesLines(tile_cache[i]->sprite, zoom, tile_x, tile_y);
            }
        }
    }
}

void drawTileCache(sprite_struct *tile_cache[], str_pxl_coords &pxl_coords)
{
    int i, offset_x, offset_y;
#ifdef __printTileCache__
    ESP_LOGD("pushTileCache", "pxl_coords=(%d,%d) which is tile=(%d,%d), idx_on_tile=(%d,%d)\n", pxl_coords.idx_x, pxl_coords.idx_y, pxl_coords.idx_x / TILE_SIZE, pxl_coords.idx_y / TILE_SIZE, pxl_coords.idx_x % TILE_SIZE, pxl_coords.idx_y % TILE_SIZE);
#endif
    for (int i_y = 0; i_y < n_sprite_y; i_y++) // Loop over the height of tiles (3)
    {
        for (int i_x = 0; i_x < n_sprite_x; i_x++) // Loop over the width of tiles (3)
        {
            i = n_sprite_x * i_y + i_x;

            offset_x = tile_cache[i]->tile_x * TILE_SIZE - pxl_coords.pxl_x + lcd.width() / 2;  // Take tile x (0 coord) - coord x (bigger than 0) and Add width -> Offset_x
            offset_y = tile_cache[i]->tile_y * TILE_SIZE - pxl_coords.pxl_y + lcd.height() / 2; // Take tile y (0 coord) - coord y(bigger than 0) and Add height -> Offset_y

            tile_cache[i]->sprite->pushSprite(offset_x, offset_y);
            ESP_LOGD("pushTileCache", "  i:%i, tile=(%d,%d), offset=(%d,%d) bufferlength=(%d)", i, tile_cache[i]->tile_x, tile_cache[i]->tile_y, offset_x, offset_y, tile_cache[i]->sprite->readPixel(0, 0));
        }
    }
}
