#include <SD.h>
#include <M5Unified.h>
#include <M5GFX.h>

#include "globalVariables.h"

static constexpr const gpio_num_t SDCARD_CSPIN = GPIO_NUM_4;

M5GFX lcd;
LGFX_Sprite canvas(&lcd); // screen buffer // Global

sprite_struct *tile_cache[n_sprite];

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
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setTextSize(4);
    M5.Lcd.println("M5Stack Swiss Topo");
    M5.Lcd.println("v0.1");
    M5.Lcd.println("by @oli4wolf");
    M5.Lcd.println("2024-01-12 20:54:02");
    delay(5000);
}

void setup()
{
    // Initialize M5Stack
    initializeM5Stack();

    // Initialise RTC
    M5.Rtc.setDateTime({{2022, 01, 12}, {20, 54, 02}});

    // Initialize SD Card
    initializeSDCard();

    // Initialize display
    startupScreen();
}

void loop()
{
}
