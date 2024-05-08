#ifndef TONE_H
#define TONE_H

// Include any necessary libraries or headers
#include "globalVariables.h"

// Declare any global constants or macros
const int frequency[12] ={200,293,369,440,475,493,550,763,985,1234,1517,1800};
const int duration[12] ={200,200,200,200,600,600,550,500,400,310,250,200};
const double climb[12] ={-10.0,-3.0,-2.0,-1.0,-0.5,0.0,0.5,1.0,2.0,3.0,5.0,10.0};
const double duty[12]= {1.0,1.0,1.0,1.0,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};

// Declare any necessary classes or structs

// Declare any function prototypes
void playTone(int tone, int duration);
void playToneByClimb(double sink_ms);

#endif // TONE_H
