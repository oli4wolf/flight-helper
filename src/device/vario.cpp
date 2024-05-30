#include "vario.h"

// Initialize Variables
MS5637 barometricSensor;
float last_pressure_reading = 0.0;
float last_altitude = 0;

/* climb rate in cm/s */
int16_t climb_cms = 0; // Global

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
        if (pressureIsOk(last_pressure_reading))
        {
            kalmanFilter.update(last_pressure_reading);
#ifdef __printVarioReadout__
            ESP_LOGD("Climb", "Last Reading: %f", last_pressure_reading);
#endif
#ifdef __logclimb__
            logClimbAltMS(pressure);
#endif
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

// Todo: Delete later when build successful.
void varioAverageLoop(void *pvParameters)
{
    while (true)
    {
        double filtered = kalmanFilter.get();
        float altitude = 44330.0 * (1.0 - pow(filtered / 1013.25, 1.0 / 5.255));
        ESP_LOGD("Climb", "last_altitude: %f, Filtered: %f, altitude: %f", last_altitude, filtered, altitude);
        climb_cms = (int16_t)((altitude - last_altitude) * 100);
        last_altitude = altitude;

        // tone(26, frequency[i], duration[i]);
        // vTaskDelay(duty[i] * (duration[i]));
        drawVarioInfo();

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
