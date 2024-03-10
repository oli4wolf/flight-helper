#ifndef TASK_H
#define TASK_H

#include "globalVariables.h"

#include "device/gps.h"
#include "device/vario.h"

// Define task function prototypes
void initGPSTask();
void initClockTask();
void initVarioTask();

#endif // TASK_H