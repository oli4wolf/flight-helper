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

// GPS variables
extern TaskHandle_t Task_GPS_read;

// By getting out the single values we decouple from the tinygpsplus library.
extern float gpsLatitude;  // GPS Latitude measurement.
extern float gpsLongitude; // GPS Longitude measurement.
extern float gpsAltitude;  // GPS Altitude measurement.
extern double gpsSpeed;    // GPS Speed measurement.
extern uint8_t gpsHours;   // Time of fix in ms.
extern uint8_t gpsMinutes; // Time of fix in ms.
extern uint8_t gpsSeconds; // Time of fix in ms.
extern double gpsDegree;   // GPS Course measurement.
extern bool gpsActive;     // GPS is active.
extern bool gpsValid;      // GPS is valid (not older than n-Seconds).

// Vario variables
extern int16_t climb_cms; // Vario climb rate in cm/s.