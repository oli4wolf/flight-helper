#include <stdio.h>
#include <M5Unified.h>
#include "coordStruct.h"

extern SemaphoreHandle_t semDrawScreen; // Initialised in main to avoid conflict SD and Display.

extern LGFX_Sprite canvas;
extern M5GFX lcd;

extern int tile_size;

extern int zoom;
extern str_pxl_coords curr_gps_pxl_coords;

extern str_pxl_coords last_drawn_pxl_coords;

// GPS variables
extern TaskHandle_t Task_GPS_read;

extern gps_data_t gps_data;
extern bool gpsActive;        // GPS is active.
extern bool gpsValid;        // GPS is valid (not older than n-Seconds).
extern bool gps_mode;        // GPS mode is active. if scrolling then deactivate GPS mode. Reactivate with button.

// Vario variables
extern int16_t climb_cms; // Vario climb rate in cm/s.