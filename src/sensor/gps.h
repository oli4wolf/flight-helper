#ifndef GPS_H
#define GPS_H

#include "globalVariables.h"
#include "storage.h" // used for logging of measurement.
#include "coordStruct.h"
#include "calculate/calcCoords.h"
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

extern TinyGPSPlus gps;
extern HardwareSerial ss;

static const uint32_t GPSBaud = 9600;
// Port C https://docs.m5stack.com/en/base/m5go_bottom2};
static const int8_t RXPin = 13;
static const int8_t TXPin = 14;

// declared twice, Todo: refactoring needed.
void initGPS();
void gpsSmartDelay(unsigned long ms);
void printGPSInfo();
void loopGPSIDX();

void gpsSmartDelay(unsigned long ms);

#endif // GPS_H