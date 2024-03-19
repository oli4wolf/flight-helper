#include "gps.h"

#include "device/gpsTestData.h"

#define TIME_ZONE (+1)   // Central European Time
#define YEAR_BASE (2022) // date in GPS starts from 2022
static const char *TAG = "gps_demo";

TinyGPSPlus gps;
HardwareSerial ss(2);

// Variables [GPS]
float gpsLatitude = 0.0;  // GPS Latitude measurement.
float gpsLongitude = 0.0; // GPS Longitude measurement.
float gpsAltitude = 0.0;  // GPS Altitude measurement.
double gpsSpeed = 0.0;    // Global.
uint8_t gpsHours;         // Global.
uint8_t gpsMinutes;       // Global.
uint8_t gpsSeconds;       // Global.
double gpsDegree = 0.0;   // Global.
bool gpsActive = false;
bool gpsValid = false;

int tile_size = 256;

int loopCnt = 0;
int drawAllXSeconds = 5;
int drawCount = 0;
bool isTimeSet = false;
void Task_GPS_read_core0(void *pvParameters)
{
  /* NMEA parser configuration */
  nmea_parser_config_t config = NMEA_PARSER_CONFIG_DEFAULT();
  /* init NMEA parser library */
  nmea_parser_handle_t nmea_hdl = nmea_parser_init(&config);

  nmea_parser_add_handler(nmea_hdl, gps_event_handler, NULL);
  for (;;)
  {
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
  ESP_LOGD("printGPSInfo", "lat/lon:%f/%f, kmph: %f, deg:%f", gpsLatitude, gpsLongitude, gpsSpeed, gpsDegree);

  char buff[100];
  snprintf(buff, sizeof(buff), "%d:%d:%d %f, %f, %f, %f", M5.Rtc.getTime().hours, M5.Rtc.getTime().minutes, M5.Rtc.getTime().seconds, gpsLatitude, gpsLongitude, gpsSpeed, gpsAltitude);
  logger(buff, "/gps.csv");
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

/**
 * @brief GPS Event Handler
 *
 * @param event_handler_arg handler specific arguments
 * @param event_base event base, here is fixed to ESP_NMEA_EVENT
 * @param event_id event id
 * @param event_data event specific arguments
 */
void gps_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
  gps_t *gps = NULL;
  switch (event_id)
  {
  case GPS_UPDATE:
    gps = (gps_t *)event_data;
    /* print information parsed from GPS statements */
    ESP_LOGI("printGPSInfo", "Satellites: %d, hdop: %d, lat/lon:%f/%f, kmph: %f, deg:%f", gps->sats_in_use, gps->dop_h, gps->latitude, gps->longitude, gps->speed);

    // By getting out the single values we decouple from the tinygpsplus library.
    if (gps->fix == GPS_FIX_GPS)
    {
      calcCoordsToCoordsPxl(curr_gps_pxl_coords, gps->latitude,
                            gps->longitude, zoom, tile_size);
      gpsSpeed = gps->speed;         // GPS Speed measurement.
      gpsHours = gps->tim.hour;      // Time of fix in ms.
      gpsMinutes = gps->tim.minute;  // Time of fix in ms.
      gpsSeconds = gps->tim.second;  // Time of fix in ms.
      gpsDegree = gps->cog;          // GPS Course measurement.
      gpsLatitude = gps->latitude;   // GPS Latitude measurement.
      gpsLongitude = gps->longitude; // GPS Longitude measurement.
      gpsAltitude = gps->altitude;

      gpsActive = true; // GPS is active.
      gpsValid = true;  // GPS is valid (not older than n-Seconds).

      if (!isTimeSet)
      {
        M5.Rtc.setDateTime({{gps->date.year, gps->date.month, gps->date.day},
                            {gps->tim.hour, gps->tim.minute, gps->tim.second}});
        isTimeSet = true;
      }
#ifdef __loggps__
      printGPSInfo(); // Also Log Data.
#endif
    }

    break;
  case GPS_UNKNOWN:
    /* print unknown statements */
    // ESP_LOGD(TAG, "Unknown statement:%s", (char *)event_data);
    break;
  default:
    break;
  }
}
