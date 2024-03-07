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