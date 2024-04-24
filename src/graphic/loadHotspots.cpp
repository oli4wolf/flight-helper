#include "loadHotspots.h"

char thermal_file_path[40];
const char thermal_dir_path[] = "/thermik";

class Hotspot
{
    // Class definition goes here
public:
    int height;
    int probability;
    std::string type;
    int pxl_x;
    int pxl_y;

    Hotspot(int height, int probability, std::string type, int pxl_x, int pxl_y)
    {
        this->pxl_x = pxl_x;
        this->pxl_y = pxl_y;
        this->height = height;
        this->probability = probability;
        this->type = type;
    }
};

Hotspot parseHotspot(const std::string &line)
{
    int pos = 0;
    int indexCPos = 1;
    int cPos[5] = {0, -1, -1, -1, -1};

    while (line.find(',', pos) != -1)
    {
        cPos[indexCPos] = line.find(',', pos);
        pos = line.find(',', pos) + 1;
        indexCPos++;
    }

    ESP_LOGD("loadHotspot", "%s", line.substr(cPos[0], cPos[1]).c_str());
    ESP_LOGD("loadHotspot", "%s", line.substr(cPos[1] + 1, cPos[2] - cPos[1]).c_str());
    ESP_LOGD("loadHotspot", "%s", line.substr(cPos[2] + 1, cPos[3] - cPos[2]).c_str());
    ESP_LOGD("loadHotspot", "%s", line.substr(cPos[3] + 1, cPos[4] - cPos[3]).c_str());
    ESP_LOGD("loadHotspot", "%s", line.substr(cPos[4] + 1, line.size() - cPos[4]).c_str());

    Hotspot hotspot = Hotspot(
        atoi(line.substr(cPos[0], cPos[1]).c_str()),
        atoi(line.substr(cPos[1] + 1, cPos[2] - cPos[1]).c_str()),
        line.substr(cPos[2] + 1, cPos[3] - cPos[2]),
        atoi(line.substr(cPos[3] + 1, cPos[4] - cPos[3]).c_str()),
        atoi(line.substr(cPos[4] + 1, line.size() - cPos[4]).c_str()));
    return hotspot;
}

// Method to display hotspots on the screen
void displayHotspots(LGFX_Sprite *sprite, Hotspot hotspot)
{
    ESP_LOGD("displayHotspots", "Hotspot: %d, %d", hotspot.pxl_x, hotspot.pxl_y);
    // Implementation goes here
    sprite->fillCircle(hotspot.pxl_x, hotspot.pxl_y, 20, TFT_YELLOW);
}

void loadHotspots(LGFX_Sprite *sprite, int zoom, int tile_x, int tile_y)
{
    int point_x, point_y;

    snprintf(thermal_file_path, 40, "%s/%d/%d/%d_hotspot.dat", thermal_dir_path, zoom,
             tile_x, tile_y);

    ESP_LOGD("loadHotspots", "Path to points: %s", thermal_file_path);
    if (SD.exists(thermal_file_path))
    {
        File fp = SD.open(thermal_file_path, FILE_READ);
        if (fp.size() > 0)
        {
            ESP_LOGD("loadHotspots", "Point dat was detected.  %s", thermal_file_path);
            while (fp.available())
            {
                std::string line = fp.readStringUntil('\n').c_str();
                ESP_LOGD("loadHotspots", "Reading line %s", line.c_str());
                Hotspot hotspot = parseHotspot(line);
                ESP_LOGD("loadHotspots", "Hotspot: %d, %d", hotspot.pxl_x, hotspot.pxl_y);
                displayHotspots(sprite, hotspot);
            }
        } else {
            //17045/11568
            if (tile_x == 17045 && tile_y == 11568){
            ESP_LOGI("loadHotspots", "Invalid Hotspot dat:  %s", thermal_file_path);
            }
        }
    }
}
