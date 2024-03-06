#include "drawGPS.h"

/**
 * @brief Draw the GPS information on the screen.
 * Decision: If a tile reload is happening it will be overdrawn but at the next valid coordination redraw.
 * Decision: Same if the Tiles are loaded from the SD-Card.
*/
void drawGPSInfo()
{
    if (xSemaphoreTake(semDrawScreen, (TickType_t)10) == pdTRUE)
    {
        uint32_t rtc_millis = M5.Rtc.getTime().hours * 3600000 + M5.Rtc.getTime().minutes * 60000 + M5.Rtc.getTime().seconds * 1000;
        uint32_t gps_time = gpsHours * 3600000 + gpsMinutes * 60000 + gpsSeconds * 1000;
        ESP_LOGD("loop", "rtc_millis: %d, gps_millis: %d", rtc_millis, gps_time);
        lcd.startWrite();
        canvas.setTextSize(2);
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
}
