#include <SD.h>
#include <M5Unified.h>
#include <M5GFX.h>

#include "globalVariables.h"

#include "graphic/tileCache.h"
#include "device/gps.h"
#include "Arduino.h"
#include "graphic/drawing.h" // Todo: might bundle the drawing logics in one.
#include "graphic/task.h"
#include "action/action.h"

#include "calculate/nmea_parser.h"

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
    lcd.setTextSize(1);
    lcd.println("M5Stack Swiss Topo");
    lcd.println("v0.1");
    lcd.println("by @oli4wolf on github");
    lcd.println("2024-01-12 20:54:02");
    lcd.println("All the maps and data come from SwissTopo.");
    lcd.println("Inspiration: https://github.com/akchan/cycle_navi");
    lcd.println("To generate SD card content: https://github.com/oli4wolf/swisstopo-tile-downloader");
    lcd.println("This is a non-commercial project.");
    delay(5000);
}

void drawMap()
{
    reloadTileCache();
    drawTileCache(tile_cache, curr_gps_pxl_coords);
    drawGPSInfo(); // Added to refresh the GPS Info with the tiles.
    pushDirIcon();
    drawVarioInfo();
    drawInfoAndMode();

    if (xSemaphoreTake(semDrawScreen, (TickType_t)10) == pdTRUE)
    {
        lcd.startWrite();
        canvas.pushSprite(0, 0);
        lcd.endWrite();
        xSemaphoreGive(semDrawScreen);
    }
    else
    {
        ESP_LOGI("drawMap", "Could not take semaphore for Drawing.");
    }
}

void setup()
{
    // xSemaphoreGive(semDrawScreen); //Free for first usage.

    // Initialize M5Stack
    initializeM5Stack();

    // Initialise RTC
    M5.Rtc.setDateTime({{2022, 01, 12}, {20, 54, 02}});

    // Initialize SD Card
    initializeSDCard();

// Initialize GPS Task
#ifdef __gps__
    initGPSTask();
#endif

// Initialize Pressure Sensor
#ifdef __vario__
    initVario();
    // Initialize Vario Readout Task reading the pressure out (this time scheduled task.)
    initVarioReadoutTask();
    // Initialize Vario Average Task reading the pressure out (this time scheduled task.)
    initVarioAverageTask();
#endif

    // Initialize direction icon
    initDirectionIcon();

    // Initialize display
    startupScreen();

    // Initialize Canvas
    initCanvas();

    // Initialize Tile Cache
    initTileCache();

    // Load the cache with the tiles.
    fillTileCache(46.93979, 7.43599);

    drawTileCache(tile_cache, curr_gps_pxl_coords);

    // Start the clock Task.
    initClockTask();
}

// Main Loop uses Xtensa::Core1
void loop()
{
    // if change then reposition screen that gps coord is in the middle
    drawMap();

    // Commands
    determineAction();

    if (gps_mode == true)
    {
        delay(1000);
    }
    else
    {
        //delay(50);
    }
}
