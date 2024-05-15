#include "vario.h"

// Initialize Variables
MS5637 barometricSensor;
float last_pressure_reading = 0.0;

int32_t climb_buffer[CLIMB_SAMPLES];
volatile uint8_t climb_buf_idx;
volatile int32_t d1, d2;

/* climb rate in cm/s */
int16_t climb_cms = 0; // Global

// Let's calculate a window depending how fast the trigger is.
int8_t counter = 0;
int average_altitude = 0;

void varioReadoutLoop(void *pvParameters)
{
    last_pressure_reading = barometricSensor.getPressure(); // Initialize it to not be 0.0
    while (gps_mode == true)                                // Debug if that deactivates the load for the vario to increase the performance.
    {
        float pressure = barometricSensor.getPressure();

        if (!barometricSensor.isConnected())
        {
            ESP_LOGE("Climb", "Barometric Sensor is not Connected!");
        }
        else
        {
            // Validation code
            if (abs(last_pressure_reading - pressure) > pressure / 3)
            {
                ESP_LOGW("Climb", "Error: Pressure: %f, temperature: %f", pressure, barometricSensor.getTemperature());
                return;
            }
            else
            {
                last_pressure_reading = pressure;
            }

            // add new value to buffer
            volatile int32_t alt_cm = ((1.0f - pow(pressure / 1013.25f, 0.190295f)) * 145366.45f) * 30.48f;
#ifdef __printVarioReadout__
            ESP_LOGD("Climb", "Pressure: %f, alt_cm: %d", pressure, alt_cm);
#endif
#ifdef __logclimb__
            logClimbAltMS(pressure, alt_cm);
#endif

            {
                climb_buffer[climb_buf_idx] = alt_cm;
                if (++climb_buf_idx >= CLIMB_SAMPLES)
                {
                    climb_buf_idx = 0;
                }
            }
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

void varioAverageLoop(void *pvParameters)
{
    while (true)
    {
        climb_cms = climb_update();
        // Climb
        // Precision of the MS5637 is about 13cm.
        if (abs(climb_cms) > 500)
        {
            double sink_ms = (climb_cms * 2.0) / 100.0;
#ifdef __sound__
            playToneByClimb(sink_ms);
#endif
        }
        drawVarioInfo();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

// Initalize pressure sensor.
void initVario()
{
    // Initialize Barometer
    if (barometricSensor.begin() == false)
    {
        Serial.println("MS5637 sensor did not respond. Please check wiring.");
        while (1)
            ;
    }
}

// Calculate the average of the last 38 pressure readings.
/* we are using linear regression here */
/* Code used from the tweety project.
 * there are two main variant as option -> Kalman Filter or Linear Regression.
 * Both are valid to be used, i have to dig deeper the advantage of the different method.
 * First i want to make it work.
 */
int16_t climb_update()
{
    /* compute average altitude */
    int32_t avg_alt = 0;

    for (uint8_t i = 0; i < CLIMB_SAMPLES; i++)
    {
        portDISABLE_INTERRUPTS();
        volatile int_fast32_t buf = climb_buffer[i];
        portENABLE_INTERRUPTS();
        avg_alt += buf;
    }

    avg_alt /= CLIMB_SAMPLES;

    /* compute LR numerator */
    int32_t LR_num = 0;
    uint8_t idx = climb_buf_idx;

    for (uint8_t i = 0; i < CLIMB_SAMPLES; i++)
    {
        /* first increasing, gives us a little time buffer */
        if (++idx >= CLIMB_SAMPLES)
            idx = 0;
        portDISABLE_INTERRUPTS();
        volatile int_fast32_t buf = climb_buffer[idx];
        portENABLE_INTERRUPTS();
        LR_num += ((int32_t)i - LR_x_cross) * (buf - avg_alt);
    }

    /* final climb value */
    volatile int_fast16_t climb = (LR_num * CLIMB_SAMPLES_PER_SEC) / LR_den;

    /* make publicly available */
    portDISABLE_INTERRUPTS();
    climb_cms = climb;
    portENABLE_INTERRUPTS();
    ESP_LOGD("Climb", "Climb in cms: %d", climb_cms);
    return climb_cms;
}
