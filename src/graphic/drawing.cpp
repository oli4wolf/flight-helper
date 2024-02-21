#include "drawing.h"

SemaphoreHandle_t semDrawScreen = xSemaphoreCreateMutex();

// Implement the functions defined in drawing.h here
void drawLineThickness(LGFX_Sprite *sprite, int x1, int x2, int y1, int y2, int color)
{
    // Bruteforce thickness.
    // Each permutation around the end and starting point is drawn. +1, -1.
    sprite->drawLine(x1 - 1, y1 - 1, x2 - 1, y2 - 1, color); //-1,-1
    sprite->drawLine(x1, y1 - 1, x2, y2 - 1, color);         // 0,-1
    sprite->drawLine(x1 + 1, y1 - 1, x2 + 1, y2 - 1, color); // 1,-1

    sprite->drawLine(x1 - 1, y1, x2 - 1, y2, color); //-1,0
    // sprite->drawLine(x1, y1, x2, y2, TFT_MAGENTA); //0,0
    sprite->drawLine(x1 + 1, y1, x2 + 1, y2, color); // 1,0

    sprite->drawLine(x1 - 1, y1 + 1, x2 - 1, y2 + 1, color); //-1,1
    sprite->drawLine(x1, y1 + 1, x2, y2 + 1, color);         // 0,1
    sprite->drawLine(x1 + 1, y1 + 1, x2 + 1, y2 + 1, color); // 1,1
}

void drawGPSInfo()
{
    uint32_t rtc_millis = M5.Rtc.getTime().hours * 3600000 + M5.Rtc.getTime().minutes * 60000 + M5.Rtc.getTime().seconds * 1000;
    uint32_t gps_time = gpsHours * 3600000 + gpsMinutes * 60000 + gpsSeconds * 1000;
    ESP_LOGD("loop", "rtc_millis: %d, gps_millis: %d", rtc_millis, gps_time);
    lcd.startWrite();
    if ((rtc_millis - gps_time) > 5000)
    {
        canvas.setTextColor(TFT_RED, TFT_BLACK);
    }
    else
    {
        canvas.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    canvas.setCursor(160, 225);
    canvas.printf("%02d:%02d:%02d, %.2f", gpsHours, gpsMinutes, gpsSeconds, gpsSpeed);
    lcd.endWrite();
    xSemaphoreGive(semDrawScreen);
}