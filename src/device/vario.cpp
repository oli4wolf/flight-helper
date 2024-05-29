#include "vario.h"

// Initialize Variables
MS5637 barometricSensor;
float last_pressure_reading = 0.0;
int last_altitude_reading = 0;

double climb_buffer[2 * CLIMB_SAMPLES]; // doubled the size of the array.
uint8_t climb_buf_idx;

/* climb rate in cm/s */
int16_t climb_cms = 0; // Global

// Let's calculate a window depending how fast the trigger is.
int average_altitude = 0;

KalmanFilter kalmanFilter = KalmanFilter();

bool pressureIsOk(double pressure)
{
    return pressure > 300. && pressure < 1250.;
}

void varioReadoutLoop(void *pvParameters)
{
    while (true)
    {
        last_pressure_reading = barometricSensor.getPressure();
        ESP_LOGD("Climb", "Last Reading: %f", last_pressure_reading);
        if (pressureIsOk(last_pressure_reading))
        {
            kalmanFilter.update(last_pressure_reading);
#ifdef __printVarioReadout__
            ESP_LOGD("Climb", "Last Reading: %f, alt_cm: %d", last_pressure_reading, alt_cm);
#endif
#ifdef __logclimb__
            logClimbAltMS(pressure);
#endif
            double filtered = kalmanFilter.get();
            climb_buffer[climb_buf_idx] = filtered;
            climb_buf_idx = climb_buf_idx + 1;
            float altitude = 44330.0 * (1.0 - pow(filtered / filtered, 1.0 / 5.255));
            ESP_LOGD("Climb", "Last_Reading: %f, Filtered: %f, altitude: %f, climb_buf_idx: %d", last_pressure_reading, filtered, altitude, climb_buf_idx);
            climb_cms = (int16_t)altitude;
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

// Todo: Delete later when build successful.
void varioAverageLoop(void *pvParameters)
{
    while (true)
    {
        if (climb_buf_idx > 0)
        {
            // Calculate average.
            for (int i = 0; i < climb_buf_idx; i++)
            {
                average_altitude = average_altitude+climb_buffer[i];
            }
            ESP_LOGD("Climb", "Average_altitude: %f, climb_buf_idx: %d", average_altitude, climb_buf_idx);
            average_altitude = average_altitude/climb_buf_idx;
            if (last_altitude_reading == 0){
                last_altitude_reading = average_altitude;
            }else if (climb_cms > 0){
                climb_cms = (last_altitude_reading - average_altitude);
            }
            ESP_LOGD("Climb", "Climb: %d, Average_altitude: %f, last_altitude_reading: %d", climb_cms, average_altitude, last_altitude_reading);
            last_altitude_reading = average_altitude;
            climb_buf_idx = 0;
            // tone(26, frequency[i], duration[i]);
            // vTaskDelay(duty[i] * (duration[i]));
            drawVarioInfo();
        }
        vTaskDelay(500);
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
