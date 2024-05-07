#include "action/tone.h"

void playTone(int tone, int duration)
{
    M5.Speaker.tone(tone, duration);
}

void playToneByClimb(double climb_cm)
{
    double sink_ms = (climb_cm * 2.0) / 100.0;
    for (int i = 0; i < 12; i++)
    {
        if (climb[i] > sink_ms)
        {
            playTone(frequency[i], duration[i]);
            break;
        }
    }
}