#ifndef VARIO_H
#define VARIO_H

#include "SparkFun_MS5637_Arduino_Library.h"
#include "globalVariables.h"
#include "graphic/drawing.h"
#include "calculate/KalmanFilter.h"  // Alternative would be a linear regression.

#define CLIMB_SAMPLES_PER_SEC		25	
#define CLIMB_AVG_SEC			1
#define CLIMB_SAMPLES			38
#define LR_x_cross 			(CLIMB_SAMPLES>>1)
#define LR_n				((int32_t)CLIMB_SAMPLES)
#define LR_x				((int32_t)LR_x_cross)
#define LR_den				((LR_n*(LR_n+1)*(2*LR_n+1))/6 - LR_n*LR_n - LR_n*LR_n*LR_x + LR_n*LR_x + LR_n*LR_x*LR_x) // LR Denominator Formula.

void initVario();
void varioReadoutLoop(void *pvParameters);
void varioAverageLoop(void *pvParameters);

const double climb[12] ={-10.0,-3.0,-2.0,-1.0,-0.5,0.0,0.5,1.0,2.0,3.0,5.0,10.0};
const int frequency[12] ={200,293,369,440,475,493,550,763,985,1234,1517,1800};
const int duration[12] ={200,200,200,200,600,600,550,500,400,310,250,200};
const double duty[12]= {1.0,1.0,1.0,1.0,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};

#endif // VARIO_H