#include "graphics.h"

#include <stdlib.h>
#include <assert.h>

const int g_screen_width = 800;
const int g_screen_height = 600;
Color* g_pixel_buffer;

void initialize_graphics(void)
{
  g_pixel_buffer = (Color*)malloc(g_screen_width * g_screen_height * sizeof(Color));
}

void deinitialize_graphics(void)
{
  free(g_pixel_buffer);
}

void put_pixel(int x, int y, Color color)
{
  assert(x >= 0 && x < g_screen_width);
  assert(y >= 0 && y < g_screen_height);
  g_pixel_buffer[x + y * g_screen_width] = color;
}

void clear(Color color)
{
  for (int y = 0; y < g_screen_height; y++) {
    for (int x = 0; x < g_screen_width; x++) {
      put_pixel(x, y, color);
    }
  }
}

void draw_triangle(const Vec3* v0, const Vec3* v1, const Vec3* v2, Color color)
{}
