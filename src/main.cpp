#include <SD.h>
#include <M5Unified.h>
#include <M5GFX.h>

#include "globalVariables.h"

#include "graphic/tileCache.h"
#include "device/gps.h"
#include "Arduino.h"
#include "graphic/drawGPS.h" // Todo: might bundle the drawing logics in one.

static constexpr const gpio_num_t SDCARD_CSPIN = GPIO_NUM_4;

M5GFX lcd;
SemaphoreHandle_t semDrawScreen = xSemaphoreCreateMutex();

void initializeM5Stack()
{
    auto cfg = M5.config();
    cfg.serial_baudrate = 115200;
    cfg.internal_imu = true;  // default=true. use internal IMU.
    cfg.internal_rtc = true;  // default=true. use internal RTC.
    cfg.internal_spk = true;  // default=true. use internal speaker.
    cfg.internal_mic = false; // default=true. use internal microphone.
    cfg.external_imu = false; // default=false. use Unit Accel & Gyro.
    cfg.external_rtc = false; // default=false. use Unit RTC.

    M5.begin(cfg);
    lcd.init();
    M5.In_I2C.release();
}

void initializeSDCard()
{
    if (!SD.begin(SDCARD_CSPIN, SPI, 20000000))
    { // Initialize the SD card.
        M5.Lcd.println(
            "Card failed, or not present");
        while (1)
            ;
    }
    ESP_LOGI("Setup()", "TF card initialized.");
}

void startupScreen()
{
    lcd.fillScreen(TFT_BLACK);
    lcd.setCursor(0, 0);
    lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    lcd.setTextSize(2);
    lcd.println("M5Stack Swiss Topo");
    lcd.println("v0.1");
    lcd.println("by @oli4wolf");
    lcd.println("2024-01-12 20:54:02");
    delay(5000);
}

void drawMap()
{
    reloadTileCache();
    drawTileCache(tile_cache, curr_gps_pxl_coords);
    drawGPSInfo();

    if (xSemaphoreTake(semDrawScreen, (TickType_t)10) == pdTRUE)
    {
    lcd.startWrite();
    canvas.pushSprite(0, 0);
    lcd.endWrite();
    xSemaphoreGive(semDrawScreen);
    }else{
        ESP_LOGW("drawMap", "Could not take semaphore for Drawing.");
    }
}

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

void setup()
{
    //xSemaphoreGive(semDrawScreen); //Free for first usage.

    // Initialize M5Stack
    initializeM5Stack();

    // Initialise RTC
    M5.Rtc.setDateTime({{2022, 01, 12}, {20, 54, 02}});

    // Initialize SD Card
    initializeSDCard();

    // Initialize GPS
    initGPS();

    // Initialize GPS Task
    initGPSTask();

    // Initialize display
    startupScreen();

    // Initialize Canvas
    initCanvas();

    // Initialize Tile Cache
    initTileCache();

    // Load the cache with the tiles.
    fillTileCache(46.95234, 7.45282);

    drawTileCache(tile_cache, curr_gps_pxl_coords);
}

// Main Loop uses Xtensa::Core1
void loop()
{
    // if change then reposition screen that gps coord is in the middle
    drawMap();

    delay(1000);
}
