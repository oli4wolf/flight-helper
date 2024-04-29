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
        if (xSemaphoreTake(semDrawScreen, (TickType_t)10) == pdTRUE)
        {
            lcd.startWrite();
            lcd.setCursor(120, 0);
            lcd.setTextColor(TFT_WHITE, TFT_BLACK);
            lcd.setTextSize(1);
            lcd.println("Zoom Level change.");
            canvas.pushSprite(0, 0);
            lcd.endWrite();
            xSemaphoreGive(semDrawScreen);
        }
        ESP_LOGI("Button", "Zoom Level change.");
        changeZoomLevel();
    }
    if (M5.BtnB.wasPressed())
    {
        if (xSemaphoreTake(semDrawScreen, (TickType_t)10) == pdTRUE)
        {
            lcd.startWrite();
            lcd.setCursor(120, 0);
            lcd.setTextColor(TFT_WHITE, TFT_BLACK);
            lcd.setTextSize(1);
            lcd.println("Recenter.");
            canvas.pushSprite(0, 0);
            lcd.endWrite();
            xSemaphoreGive(semDrawScreen);
        }
        ESP_LOGI("Button", "Recenter Map.");
        gps_mode = true;
    }
    if (M5.BtnC.wasPressed())
    {
        ESP_LOGI("Button", "Hikingpath.");
        hike_mode = !hike_mode;
    }

    checkTouchMoveEvent();
}

#endif // ACTION_H