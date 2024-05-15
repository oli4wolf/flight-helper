#ifndef VARIO_H
#define VARIO_H

#include "SparkFun_MS5637_Arduino_Library.h"
#include "globalVariables.h"
#include "graphic/drawing.h"
#include "action/tone.h"

#define CLIMB_SAMPLES_PER_SEC		50	
#define CLIMB_AVG_SEC			0.5
#define CLIMB_SAMPLES			25 // 25 samples per half second. Average trigger all 0.5 seconds.
#define LR_x_cross 			(CLIMB_SAMPLES>>1)
#define LR_n				((int32_t)CLIMB_SAMPLES)
#define LR_x				((int32_t)LR_x_cross)
#define LR_den				((LR_n*(LR_n+1)*(2*LR_n+1))/6 - LR_n*LR_n - LR_n*LR_n*LR_x + LR_n*LR_x + LR_n*LR_x*LR_x) // LR Denominator Formula.

void initVario();
void varioReadoutLoop(void *pvParameters);
void varioAverageLoop(void *pvParameters);
int16_t climb_update();

#endif // VARIO_H