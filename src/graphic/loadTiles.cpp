#include "loadTiles.h"

#define LEN_FILE_PATH 40
char file_path[LEN_FILE_PATH];
const char map_dir_path[] = "/map";

void loadTile(LGFX_Sprite *sprite, int zoom, int tile_x, int tile_y)
{
    ESP_LOGD("LoadTile", "Loading tile: %d, %d, %d", zoom, tile_x, tile_y);
    snprintf(file_path, LEN_FILE_PATH, "%s/%d/%d/%d.jpeg", map_dir_path, zoom, tile_x,
             tile_y);

    if (SD.exists(file_path))
    {
        File fp = SD.open(file_path, FILE_READ);
        if (fp.size() > 0)
        {
            uint32_t t = millis();

            sprite->drawJpgFile(SD, file_path);

            t = millis() - t;
            ESP_LOGD("LoadTile", "MapTile was updated. (%d ms) %s", t, file_path);
        }
        fp.close();
    }
    else
    {
        sprite->fillSprite(TFT_RED);
        ESP_LOGW("LoadTile", "Map was not found: %s", file_path);
    }
}
