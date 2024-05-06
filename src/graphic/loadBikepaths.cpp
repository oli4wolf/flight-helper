#include "loadBikepaths.h"

char file_path_bike[39];
const char map_dir_path_bike[] = "/bike";
bool bike_mode = false;

void loadBikepaths(LGFX_Sprite *sprite, int zoom, int tile_x, int tile_y)
{
    ESP_LOGD("Bikepath", "Loading Bikepath: %d, %d, %d", zoom, tile_x, tile_y);
    snprintf(file_path_bike, 39, "%s/%d/%d/%d.png", map_dir_path_bike, zoom, tile_x,
             tile_y);

    if (SD.exists(file_path_bike))
    {
        File fp = SD.open(file_path_bike, FILE_READ);
        if (fp.size() > 0)
        {
            uint32_t t = millis();

            sprite->drawPngFile(SD, file_path_bike);

            t = millis() - t;
            ESP_LOGD("Bikepath", "Paths was updated. (%d ms) %s", t, file_path_bike);
        }
        fp.close();
    }
    else
    {
        sprite->fillSprite(TFT_RED);
        ESP_LOGW("Bikepath", "Paths was not found: %s", file_path_bike);
    }
}
