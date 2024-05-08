#include "action/tone.h"

void playTone(int tone, int duration)
{
    M5.Speaker.tone(tone, duration);
}

void playToneByClimb(double sink_ms)
{
    for (int i = 0; i < 12; i++)
    {

        if (climb[i] > sink_ms)
        {
            ESP_LOGD("Climb", "Sink_ms: %f, frequency: %d, duration: %d, duty: %d, duration*duty: %f",
                    sink_ms, frequency[i], duration[i], duty[i], (duty[i] * (duration[i])));
            playTone(frequency[i], duration[i]);
            break;
        }
    }
}