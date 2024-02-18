#include "storage.h"

void fileWriter(char *csv, File fp)
{
  fp.println(csv);
  fp.close();
}

void logger(char *csv, const char *filePath)
{
  if (xSemaphoreTake(semDrawScreen, (TickType_t)10) == pdTRUE)
  {
    File fp;
    if (!SD.exists(filePath))
    {
      File fp = SD.open(filePath, FILE_WRITE);
    }
    else
    {
      File fp = SD.open(filePath, FILE_APPEND);
    }
    fileWriter(csv, fp);
    xSemaphoreGive(semDrawScreen);
  }
}

void logClimbAltMS(float pressure, int32_t altCM)
{
  if (xSemaphoreTake(semDrawScreen, (TickType_t)10) == pdTRUE)
  {
    File fp;
    if (!SD.exists("/climb.csv"))
    {
      File fp = SD.open("/climb.csv", FILE_WRITE);
    }
    else
    {
      File fp = SD.open("/climb.csv", FILE_APPEND);
    }
    fileWriter("/climb.csv", fp);
    xSemaphoreGive(semDrawScreen);
  }
}
