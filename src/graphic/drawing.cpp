#include "drawing.h"

M5Canvas dir_icon(&canvas);

#define DIR_ICON_COLOR_ACTIVE TFT_DARKGREEN
#define DIR_ICON_COLOR_INACTIVE TFT_DARKGREY
#define DIR_ICON_BG_COLOR TFT_WHITE
#define DIR_ICON_TRANS_COLOR TFT_BLACK
#define DIR_ICON_R 14
#define DIR_ICON_EDGE_WIDTH 3
#define DIR_ICON_ANGLE 45.0 / 180.0 * M_PI
const int dir_icon_palette_id_trans = 0;
const int dir_icon_palette_id_bg = 1;
const int dir_icon_palette_id_fg = 2;

// Implement the functions defined in drawing.h here
void drawLineThickness(LGFX_Sprite *sprite, int x1, int x2, int y1, int y2, int color)
{
    // Bruteforce thickness.
    // Each permutation around the end and starting point is drawn. +1, -1.
    sprite->drawLine(x1 - 1, y1 - 1, x2 - 1, y2 - 1, color); //-1,-1
    sprite->drawLine(x1, y1 - 1, x2, y2 - 1, color);         // 0,-1
    sprite->drawLine(x1 + 1, y1 - 1, x2 + 1, y2 - 1, color); // 1,-1

    sprite->drawLine(x1 - 1, y1, x2 - 1, y2, color); //-1,0
    // sprite->drawLine(x1, y1, x2, y2, TFT_MAGENTA); //0,0
    sprite->drawLine(x1 + 1, y1, x2 + 1, y2, color); // 1,0

    sprite->drawLine(x1 - 1, y1 + 1, x2 - 1, y2 + 1, color); //-1,1
    sprite->drawLine(x1, y1 + 1, x2, y2 + 1, color);         // 0,1
    sprite->drawLine(x1 + 1, y1 + 1, x2 + 1, y2 + 1, color); // 1,1
}

void drawGPSInfo()
{
    if (xSemaphoreTake(semDrawScreen, (TickType_t)10) == pdTRUE)
    {
        uint32_t rtc_millis = M5.Rtc.getTime().hours * 3600000 + M5.Rtc.getTime().minutes * 60000 + M5.Rtc.getTime().seconds * 1000;
        uint32_t gps_time = gps_data.hours * 3600000 + gps_data.minutes * 60000 + gps_data.seconds * 1000;
        ESP_LOGD("loop", "rtc_millis: %d, gps_millis: %d", rtc_millis, gps_time);
        lcd.startWrite();
        canvas.setTextSize(2);
        if ((rtc_millis - gps_time) > 5000)
        {
            canvas.setTextColor(TFT_RED, TFT_BLACK);
            gps_valid = false;
        }
        else
        {
            canvas.setTextColor(TFT_GREEN, TFT_BLACK);
            gps_valid = true;
        }
        canvas.setCursor(160, 225);
        canvas.printf("%02d:%02d:%02d, %.2f", gps_data.hours, gps_data.minutes, gps_data.seconds, gps_data.speed);
        canvas.pushSprite(0, 0); // needed to display the text.
        lcd.endWrite();
        xSemaphoreGive(semDrawScreen);
    }
    else
    {
        ESP_LOGI("drawGPSInfo", "Could not take semaphore for Drawing.");
    }
}

void drawVarioInfo()
{
    if (xSemaphoreTake(semDrawScreen, (TickType_t)10) == pdTRUE)
    {
        lcd.startWrite();
        canvas.setTextSize(2);
        if ((climb_cms) > 50)
        {
            canvas.setTextColor(TFT_GREEN, TFT_BLACK);
            gps_valid = false;
        }
        else if ((climb_cms) < -50)
        {
            canvas.setTextColor(TFT_RED, TFT_BLACK);
            gps_valid = false;
        }
        else
        {
            canvas.setTextColor(TFT_WHITE, TFT_BLACK);
            gps_valid = true;
        }
        canvas.setCursor(0, 225);
        canvas.printf("Climb: %d", climb_cms);
        canvas.pushSprite(0, 0); // needed to display the text.
        lcd.endWrite();
        xSemaphoreGive(semDrawScreen);
    }
    else
    {
        ESP_LOGI("drawGPSInfo", "Could not take semaphore for Drawing.");
    }
}

void drawHikeMode()
{
    if (bike_mode || hike_mode)
    {
        if (xSemaphoreTake(semDrawScreen, (TickType_t)10) == pdTRUE)
        {
            lcd.startWrite();
            canvas.setTextSize(2);
            canvas.setCursor(0, 0);
            if (hike_mode && bike_mode)
            {
                canvas.setTextColor(TFT_RED);
                canvas.printf("Hike & Bike");
            }
            else if (hike_mode)
            {
                canvas.setTextColor(TFT_YELLOW);
                canvas.printf("Hike");
            }
            else if (bike_mode)
            {
                canvas.setTextColor(TFT_BLUE);
                canvas.printf("Bike");
            }
            canvas.pushSprite(0, 0); // needed to display the text.
            lcd.endWrite();
            xSemaphoreGive(semDrawScreen);
        }
        else
        {
            ESP_LOGI("drawHikeMode", "Could not take semaphore for Drawing.");
        }
    }
}

void drawGPSInfoLoop(void *pvParameters)
{
    while (true)
    {
        drawGPSInfo();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Used the concept of akchan cycle_navi.
 *
 */
void initDirectionIcon()
{
    /*
     * dir_icon color palette:
     *   0: DIR_ICON_TRANS_COLOR
     *   1: DIR_ICON_BG_COLOR
     *   2: foreground color (DIR_ICON_COLOD_ACTIVE or DIR_ICON_COLOR_INACTIVE)
     *   3: not used (default is TFT_WHITE)
     */

    // Allocate sprite
    dir_icon.setColorDepth(2);
    dir_icon.setPsram(false);
    dir_icon.createSprite(DIR_ICON_R * 2 + 1, DIR_ICON_R * 2 + 1);

    // Set palette colors
    dir_icon.setPaletteColor(dir_icon_palette_id_trans, DIR_ICON_TRANS_COLOR);
    dir_icon.setPaletteColor(dir_icon_palette_id_bg, DIR_ICON_BG_COLOR);
    dir_icon.setPaletteColor(dir_icon_palette_id_fg, DIR_ICON_COLOR_INACTIVE);

    // Draw icon
    dir_icon.fillSprite(dir_icon_palette_id_trans); // translucent background
    dir_icon.fillCircle(DIR_ICON_R, DIR_ICON_R, DIR_ICON_R, dir_icon_palette_id_fg);
    dir_icon.fillCircle(DIR_ICON_R, DIR_ICON_R, DIR_ICON_R - DIR_ICON_EDGE_WIDTH,
                        dir_icon_palette_id_bg);

    int x0 = DIR_ICON_R;
    int y0 = DIR_ICON_EDGE_WIDTH;
    int x1 = DIR_ICON_R + (DIR_ICON_R - DIR_ICON_EDGE_WIDTH) * cos(-M_PI_2 + DIR_ICON_ANGLE);
    int y1 = DIR_ICON_R - (DIR_ICON_R - DIR_ICON_EDGE_WIDTH) * sin(-M_PI_2 + DIR_ICON_ANGLE);
    int x2 = DIR_ICON_R - (DIR_ICON_R - DIR_ICON_EDGE_WIDTH) * cos(-M_PI_2 + DIR_ICON_ANGLE);
    int y2 = DIR_ICON_R - (DIR_ICON_R - DIR_ICON_EDGE_WIDTH) * sin(-M_PI_2 + DIR_ICON_ANGLE);

    dir_icon.fillTriangle(x0, y0, x1, y1, x2, y2, dir_icon_palette_id_fg);

    x0 = DIR_ICON_R;
    y0 = (int)(DIR_ICON_R * 1.2);
    dir_icon.fillTriangle(x0, y0, x1, y1, x2, y2, dir_icon_palette_id_bg);

    // set center of rotation
    dir_icon.setPivot(DIR_ICON_R, DIR_ICON_R);
}

void pushDirIcon()
{
    double dir_degree = gps_data.degree;

    int offset_x = M5.Display.width() / 2;
    int offset_y = M5.Display.height() / 2;

    // When dir icon is out of canvas
    if (!((-DIR_ICON_R < offset_x && offset_x < M5.Display.width() + DIR_ICON_R) &&
          (-DIR_ICON_R < offset_y && offset_y < M5.Display.height() + DIR_ICON_R)))
    {
        ESP_LOGD("pushDirIcon()", "out of canvas offset=(%d,%d)\n", offset_x, offset_y);
        return;
    }

    if (gps_valid)
    {
        dir_icon.setPaletteColor(dir_icon_palette_id_fg, DIR_ICON_COLOR_ACTIVE);
    }
    else
    {
        dir_icon.setPaletteColor(dir_icon_palette_id_fg, DIR_ICON_COLOR_INACTIVE);
    }

    dir_icon.pushRotateZoomWithAA(offset_x, offset_y, dir_degree, 1, 1,
                                  dir_icon_palette_id_trans);
}