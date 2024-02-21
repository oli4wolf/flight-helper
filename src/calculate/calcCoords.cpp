#include <M5Unified.h>
#include "calcCoords.h"
#include "math.h"

// ================================================================================
// Coords calculation to Tile
// ================================================================================
int calcCoords2TileCoords(str_tile_coords &tile_coords, int zoom, double lat,
                          double lon)
{
    int method = 2;
    // 0: tan
    // 1: sin
    // 2: sin_cos
    // 3: cos_tan

    if (lat <= 90.0 && lat >= -90.0)
    {
        // valid latitude case
        double r = 1.0 / (2 * M_PI);
        double x_raw = 2 * M_PI * r * (lon + 180.0) / 360.0;

        double y_raw, rad;
        if (method == 0)
        {
            rad = M_PI * (90.0 + lat) / 360.0;
            y_raw = r * log(tan(rad));
        }
        else if (method == 1)
        {
            rad = lat / 180.0 * M_PI;
            y_raw = r / 2.0 * log((1.0 + sin(rad)) / (1.0 - sin(rad)));
        }
        else if (method == 2)
        {
            rad = lat / 180.0 * M_PI;
            y_raw = r * log((1.0 + sin(rad)) / cos(rad));
        }
        else if (method == 3)
        {
            rad = lat / 180.0 * M_PI;
            y_raw = r * log(1.0 / cos(rad) + tan(rad));
        }

        tile_coords.zoom = zoom;
        tile_coords.tile_x = (int)floor(x_raw * (1 << zoom));
        tile_coords.tile_y = (int)floor((0.5 - y_raw) * (1 << zoom));

        return 0;
    }
    else
    {
        // Invalid latitude case
        tile_coords.zoom = zoom;
        tile_coords.tile_x = -1;
        tile_coords.tile_y = -1;

        return -1;
    }
}

int calcIdxOnTile(str_pxl_on_tile &idx_on_tile, int zoom, int lat, int lon,
                  int tile_size)
{
    str_tile_coords tile_coords1, tile_coords2;
    int ret;

    ret = calcCoords2TileCoords(tile_coords1, zoom, lat, lon);
    if (ret == -1)
        return -1;

    if (tile_size <= 0)
        return -1;
    int zoom2 = zoom + (int)log2(tile_size);

    ret = calcCoords2TileCoords(tile_coords2, zoom2, lat, lon);
    if (ret == -1)
        return -1;

    idx_on_tile.pxl_x = tile_coords2.tile_x - tile_coords1.tile_x * tile_size;
    idx_on_tile.pxl_y = tile_coords2.tile_y - tile_coords1.tile_y * tile_size;

    return 0;
}

void calcCoordsIdx2Tile(str_tile_coords &tile_coords, str_pxl_coords &idx_coords,
                        int tile_size)
{
    tile_coords.tile_x = idx_coords.pxl_x / tile_size;
    tile_coords.tile_y = idx_coords.pxl_y / tile_size;
}

void calcCoordsIdx2IdxOnTile(str_pxl_on_tile &idx_on_tile,
                             str_pxl_coords &idx_coords, int tile_size)
{
    idx_on_tile.pxl_x = idx_coords.pxl_x % tile_size;
    idx_on_tile.pxl_y = idx_coords.pxl_y % tile_size;
}

int calcCoords2CoordsIdx(str_pxl_coords &idx_coords, double lat, double lon,
                         int zoom, int tile_size)
{
    str_tile_coords tile_coords;

    if (tile_size <= 0)
        return -1;
    int zoom2 = zoom + (int)log2(tile_size);

    int ret = calcCoords2TileCoords(tile_coords, zoom2, lat, lon);
    if (ret == -1)
        return -1;

    idx_coords.pxl_x = tile_coords.tile_x;
    idx_coords.pxl_y = tile_coords.tile_y;

    ESP_LOGD("calculate","Tile x,y: %d, %d");

    return 0;
}