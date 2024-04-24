#include "zoomLevel.h"

int zoom_prev = 15;

void changeZoomLevel()
{
  int zoom_prev = zoom;
  zoom++;

  if (zoom > MAX_ZOOM)
  {
    zoom = MIN_ZOOM;
  }

  ESP_LOGD("Zoom Level", "Zoom level changed from %d to %d. ", zoom_prev, zoom);

  // Update curr_gps_idx_coords for the new zoom level
  if (gps_active)
  {
    calcCoordsToCoordsPxl(curr_gps_pxl_coords, gps_data.longitude,
                         gps_data.latitude, zoom, tile_size);
  }
  else
  {
    curr_gps_pxl_coords.pxl_x = (int)(curr_gps_pxl_coords.pxl_x * pow(2.0, (double)(zoom - zoom_prev)));
    curr_gps_pxl_coords.pxl_y = (int)(curr_gps_pxl_coords.pxl_y * pow(2.0, (double)(zoom - zoom_prev)));
  }

  ESP_LOGD("Zoom Level", "Zoom %d to (%d,%d)", zoom, curr_gps_pxl_coords.pxl_x, curr_gps_pxl_coords.pxl_y);
}