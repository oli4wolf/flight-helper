#include "loadObstacles.h"

char file_path_obstacles[40];
const char point_dir_path[] = "/obstacles";

void loadObstaclesLines(LGFX_Sprite *sprite, int zoom, int tile_x, int tile_y)
{
    snprintf(file_path_obstacles, 40, "%s/%d/%d/%d_line.dat", point_dir_path, zoom,
             tile_x, tile_y);

    if (!SD.exists(file_path_obstacles))
    {
        return;
    }
    File fp = SD.open(file_path_obstacles, FILE_READ);
    if (fp.size() > 0)
    {
        // Last Point
        char lastPointId[10] = "";
        int lastPosX = -1;
        int lastPosY = -1;

        while (fp.available())
        {
            // Lines are in order therefore if the same Lineobject is in the first Parameter we can continue a line.
            std::string line = fp.readStringUntil('\n').c_str();

            char *pt;
            char *c = const_cast<char *>(line.c_str());
            pt = strtok(c, ",");

            int posX;
            int posY;
            while (pt != NULL)
            {
                char *pointId = pt;

                // lastID = pt;
                pt = strtok(NULL, ","); // order number
                pt = strtok(NULL, ","); // Type
                pt = strtok(NULL, ","); // X

                posX = atoi(pt);
                pt = strtok(NULL, ","); // Y
                posY = atoi(pt);
                pt = strtok(NULL, ",");

                sprite->fillCircle(posX, posY, 5, TFT_GOLD);

                if (strcmp(pointId, lastPointId) == 0)
                {
                    if (lastPosX != -1 && lastPosY != -1)
                    {
                        if (lastPosX != posX && lastPosY != posY)
                        {
                            drawLineThickness(sprite, lastPosX, posX, lastPosY, posY, TFT_MAGENTA);
                            sprite->drawLine(lastPosX, lastPosY, posX, posY, TFT_RED);
                        }
                    }
                }
                strncpy(lastPointId, pointId, strlen(pointId)); // Allways 10
                lastPosX = posX;
                lastPosY = posY;
            }
        }
    }
    else
    {
        ESP_LOGI("loadObstaclesLines", "Invalid Line dat:  %s", file_path_obstacles);
    }
    fp.close();
}


void loadObstaclesPoints(LGFX_Sprite *sprite, int zoom, int tile_x, int tile_y)
{
    int point_x, point_y;

    snprintf(file_path_obstacles, 40, "%s/%d/%d/%d_point.dat", point_dir_path, zoom,
             tile_x, tile_y);

    ESP_LOGD("loadObstaclesPoints", "Path to points: %s", file_path_obstacles);
    if (SD.exists(file_path_obstacles))
    {
        File fp = SD.open(file_path_obstacles, FILE_READ);
        if (fp.size() > 0)
        {
            ESP_LOGD("loadObstaclesPoints", "Point dat was detected.  %s", file_path_obstacles);
            while (fp.available())
            {

                std::string line = fp.readStringUntil('\n').c_str();
                int pos = 0;
                int indexCPos = 1;
                int cPos[5] = {0, -1, -1, -1, -1};

                while (line.find(',', pos) != -1)
                {
                    cPos[indexCPos] = line.find(',', pos);
                    pos = line.find(',', pos) + 1;
                    indexCPos++;
                }

                    ESP_LOGD("loadObstaclesPoints", "%s", line.substr(cPos[0], cPos[1]).c_str());
                    ESP_LOGD("loadObstaclesPoints", "%s", line.substr(cPos[1] + 1, cPos[2] - cPos[1]).c_str());
                    ESP_LOGD("loadObstaclesPoints", "%s", line.substr(cPos[2] + 1, cPos[3] - cPos[2]).c_str());
                    ESP_LOGD("loadObstaclesPoints", "%s", line.substr(cPos[3] + 1, cPos[4] - cPos[3]).c_str());
                    ESP_LOGD("loadObstaclesPoints", "%s", line.substr(cPos[4] + 1, line.size() - cPos[4]).c_str());
                    sprite->fillCircle(atoi(line.substr(cPos[3] + 1, cPos[4] - cPos[3]).c_str()), atoi(line.substr(cPos[3] + 1, cPos[4] - cPos[3]).c_str()), 10, TFT_RED);
            }
        }
        fp.close();
    }
    else
    {
        ESP_LOGW("loadObstaclesPoints", "Invalid point dat:  %s", file_path_obstacles);
    }
}