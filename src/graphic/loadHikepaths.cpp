#include "loadHikepaths.h"

char file_path_hike[39];
const char map_dir_path_hike[] = "/hike";
bool hike_mode = false;

void loadHikepaths(LGFX_Sprite *sprite, int zoom, int tile_x, int tile_y)
{
    ESP_LOGD("Hikepath", "Loading Hikepath: %d, %d, %d", zoom, tile_x, tile_y);
    snprintf(file_path_hike, 39, "%s/%d/%d/%d.png", map_dir_path_hike, zoom, tile_x,
             tile_y);

    if (SD.exists(file_path_hike))
    {
        File fp = SD.open(file_path_hike, FILE_READ);
        if (fp.size() > 0)
        {
            uint32_t t = millis();

            sprite->drawPngFile(SD, file_path_hike);

            t = millis() - t;
            ESP_LOGD("Hikepath", "Paths was updated. (%d ms) %s", t, file_path_hike);
        }
        fp.close();
    }
    else
    {
        sprite->fillSprite(TFT_RED);
        ESP_LOGW("Hikepath", "Paths was not found: %s", file_path_hike);
    }
}
