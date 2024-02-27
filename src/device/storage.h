#ifndef STORAGE_H
#define STORAGE_H

#include <SD.h>
#include "globalVariables.h"

extern SemaphoreHandle_t semDrawScreen;

void logger(char *csv, const char *filePath);
void logClimbAltMS(float pressure, int32_t altCM);

#endif // STORAGE_H
