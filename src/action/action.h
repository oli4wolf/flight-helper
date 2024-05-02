#ifndef ACTION_H
#define ACTION_H

#include "globalVariables.h"
#include "zoomLevel.h"
#include "scroll.h"

void determineAction()
{
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
        ESP_LOGI("Button", "Hikingpath or Bikepath.");
        hike_mode = !hike_mode;
        bike_mode = !bike_mode;
        if (hike_mode && bike_mode)
        {
            // Turn both modes off.
            hike_mode = false;
            bike_mode = false;
        }
        else if (hike_mode)
        {
            // Turn hike off and engage bike.
            hike_mode = false;
            bike_mode = true;
        }
        else if (bike_mode)
        {
           // Turn both modes off.
            hike_mode = true;
            bike_mode = true;
        }
        else
        {
            // Turn both modes off.
            hike_mode = true;
            bike_mode = false;
        }
        invalidateTileCache();
        reloadTileCache();
    }

    checkTouchMoveEvent();
}

#endif // ACTION_H