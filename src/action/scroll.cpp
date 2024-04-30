#include "scroll.h"

void recenterMap()
{
    gps_mode = true;
}

unsigned long t_last_touch_move = 0;
void checkTouchMoveEvent()
{
    auto t = M5.Touch.getDetail();

    auto dx = t.deltaX();
    auto dy = t.deltaY();

    if (t.isPressed() && (dx || dy)) // If moving
    {
        ESP_LOGD("Touch Event", " diff (x=%d,y=%d)\n", dx, dy);
        gps_mode = false;
        t_last_touch_move = millis();

        curr_gps_pxl_coords.pxl_x -= dx;
        curr_gps_pxl_coords.pxl_y -= dy;

        ESP_LOGD("Touch Event", "curr_gps_pxl_coords (x=%d,y=%d)", curr_gps_pxl_coords.pxl_x, curr_gps_pxl_coords.pxl_y);
    }
}