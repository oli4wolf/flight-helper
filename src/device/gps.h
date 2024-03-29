#ifndef GPS_H
#define GPS_H

#include "globalVariables.h"
#include "device/storage.h" // used for logging of measurement.
#include "coordStruct.h"
#include "calculate/calcCoords.h"
#include "graphic/drawing.h"
#include <HardwareSerial.h>

#include "calculate/nmea_parser.h"

// Port C https://docs.m5stack.com/en/base/m5go_bottom2};
//static const int8_t RXPin = 13;
//static const int8_t TXPin = 14;

// declared twice, Todo: refactoring needed.
void Task_GPS_read_core0(void *pvParameters);
void gpsDebugCoords();
void printGPSInfo();

void gps_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

#endif // GPS_H