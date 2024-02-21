#include <stdio.h>
#include <M5Unified.h>
#include "coordStruct.h"

extern SemaphoreHandle_t semDrawScreen; // Initialised in main to avoid conflict SD and Display.

extern LGFX_Sprite canvas;
extern M5GFX lcd;

extern int zoom;
extern str_pxl_coords curr_gps_pxl_coords;
extern str_pxl_coords display_center_pxl_coords;

extern str_pxl_coords last_drawn_pxl_coords;