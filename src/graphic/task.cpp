#include "task.h"

// Implement the functions declared in the task.h header file here

// Example function implementation
TaskHandle_t Task_GPS_read = NULL;
TaskHandle_t Task_Clock = NULL;

// init GPS Task
void initGPSTask(){
    xTaskCreatePinnedToCore(
      Task_GPS_read_core0, /* Task function. */
      "Task_GPS_read",     /* name of task. */
      4096,                /* Stack size of task */
      NULL,                /* parameter of the task */
      1,                   /* priority of the task */
      &Task_GPS_read,      /* Task handle */
      0);                  /* pin task to core 0 */
}

void initClockTask(){
    xTaskCreatePinnedToCore(
      drawGPSInfoLoop, /* Task function. */
      "Task_Clock",     /* name of task. */
      4096,             /* Stack size of task */
      NULL,             /* parameter of the task */
      0,                /* priority of the task */
      &Task_Clock,             /* Task handle */
      0);               /* pin task to core 1 */
}

// Vario Task as Scheduled task.
void initVarioTask(){

    // Needed to change it from the Espressif example .c to .cpp initialization.
    esp_timer_create_args_t periodic_timer_args = {
        .callback = &vario_readout_periodic_timer_callback,
        .name = "vario readout timer"
    };

    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));

    /* Start the timers */
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 20000)); // (1 second/CLIMB_SAMPLES_PER_SEC) to microseconds. 
}