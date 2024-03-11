#include "gps.h"

#include "device/gpsTestData.h"

TinyGPSPlus gps;
HardwareSerial ss(2);

// Variables [GPS]
double gpsSpeed = 0.0; // Global.
uint8_t gpsHours;      // Global.
uint8_t gpsMinutes;    // Global.
uint8_t gpsSeconds;    // Global.
double gpsDegree = 0.0;      // Global.
bool gpsActive = false;
bool gpsValid = false;

int tile_size = 256;

int loopCnt = 0;
int drawAllXSeconds = 5;
int drawCount = 0;
bool isTimeSet = false;
void Task_GPS_read_core0(void *pvParameters)
{
  for (;;)
  {
    while (ss.available() > 0)
      gps.encode(ss.read());
    if (gps.location.isUpdated())
    {
      ESP_LOGD("GPS_read", "lat/lon:%f/%f, kmph: %f", gps.location.lat(), gps.location.lng(), gps.speed.kmph());
      gpsActive = true;
      loopGPSIDX();
    }

    gpsDebugCoords();

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void initGPS()
{
  ss.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);
};

void printGPSInfo()
{
  gpsSpeed = gps.speed.kmph();
  gpsHours = gps.time.hour();
  gpsMinutes = gps.time.minute();
  gpsSeconds = gps.time.second();
  gpsDegree = gps.course.deg();
  ESP_LOGD("printGPSInfo", "Satellites: %d, hdop: %d, lat/lon:%f/%f, kmph: %f, deg:%f", gps.satellites.value(), gps.hdop.value(), gps.location.lat(), gps.location.lng(), gpsSpeed);

  char buff[100];
  snprintf(buff, sizeof(buff), "%d: %f, %f, %f, %f", gps.time.value(), gps.location.lat(), gps.location.lng(), gpsSpeed, gps.altitude.meters());
  logger(buff, "/gps.csv");
}

void loopGPSIDX()
{
  // Update curr_gps_idx_coords with gps data
  calcCoordsToCoordsPxl(curr_gps_pxl_coords, gps.location.lat(),
                        gps.location.lng(), zoom, tile_size);

  // Update global time variables
  gpsHours = gps.time.hour();
  gpsMinutes = gps.time.minute();
  gpsSeconds = gps.time.second();
  gpsSpeed = gps.speed.kmph();
  gpsDegree = gps.course.deg();

  if (!isTimeSet)
  {
    M5.Rtc.setDateTime({{static_cast<int8_t>(gps.date.year()), static_cast<int8_t>(gps.date.month()), static_cast<int8_t>(gps.date.day())},
                        {static_cast<int8_t>(gps.time.hour()), static_cast<int8_t>(gps.time.minute()), static_cast<int8_t>(gps.time.second())}});
    isTimeSet = true;
  }

#ifdef __loggps__
  printGPSInfo(); // Also Log Data.
#endif
}

void gpsDebugCoords()
{
// Enter Debugmode if no GPS Signal is received and activated.
#ifdef __gpsdebug__
  drawCount = drawCount + 1;
  if (gpsActive == false && (drawCount % drawAllXSeconds == 0))
  {
    // Update curr_gps_idx_coords with gps data
    calcCoordsToCoordsPxl(curr_gps_pxl_coords, data[loopCnt].lat,
                          data[loopCnt].lon, zoom, tile_size);

    loopCnt++;
    if (loopCnt >= data.size())
    {
      loopCnt = 0;
    }
  }
#endif
}