#include "scroll.h"

void recenterMap()
{
}

unsigned long t_last_touch_move = 0;
void checkTouchMoveEvent()
{
    auto t = M5.Touch.getDetail();

    auto dx = t.deltaX();
    auto dy = t.deltaY();

    if (t.isPressed() && (dx || dy)) // If moving
    {
        gps_mode = false;
        t_last_touch_move = millis();

        display_center_pxl_coords.pxl_x -= dx;
        display_center_pxl_coords.pxl_y -= dy;

        ESP_LOGD("Touch Event", "display_center_pxl_coords (x=%d,y=%d)", display_center_pxl_coords.pxl_x, display_center_pxl_coords.pxl_y);
            ESP_LOGD("Touch Event", " diff (x=%d,y=%d)\n", dx, dy);
    }
}