#ifndef CALCCOORDS_H
#define CALCCOORDS_H

#define M_PI 3.14159265358979323846 /* pi */

#include "coordStruct.h"

#endif

int calcIdxOnTile(str_pxl_on_tile &idx_on_tile, int zoom, int lat, int lon,
                  int tile_size);
void calcCoordsPxlToTile(str_tile_coords &tile_coords, str_pxl_coords &idx_coords,
                        int tile_size);
void calcCoordsIdx2IdxOnTile(str_pxl_on_tile &idx_on_tile,
                             str_pxl_coords &idx_coords, int tile_size);
int calcCoordsToCoordsPxl(str_pxl_coords &idx_coords, double lat, double lon,
                         int zoom, int tile_size);
int calcCoords2TileCoords(str_tile_coords &tile_coords, int zoom, double lat,
                          double lon);