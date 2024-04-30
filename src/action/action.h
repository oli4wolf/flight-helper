#ifndef ACTION_H
#define ACTION_H

#include "globalVariables.h"
#include "zoomLevel.h"
#include "scroll.h"

void determineAction()
{
    ESP_LOGD("Action", "determineAction");
    // Commands
    M5.update();
    if (M5.BtnA.wasPressed())
    {
        ESP_LOGI("Button", "Zoom Level change.");
        changeZoomLevel();
    }
    if (M5.BtnB.wasPressed())
    {
        ESP_LOGI("Button", "Recenter Map.");
        gps_mode = true;
    }
    if (M5.BtnC.wasPressed())
    {
        ESP_LOGI("Button", "Hikingpath.");
        hike_mode = !hike_mode;
        invalidateTileCache();
        reloadTileCache();
    }

    checkTouchMoveEvent();
}

#endif // ACTION_H