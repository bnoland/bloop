#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "vector.h"

#include <stdint.h>

typedef uint32_t Color;

typedef struct
{
  int screen_width;
  int screen_height;
  Color* pixel_buffer;
} Graphics;

Graphics graphics_make(int screen_width, int screen_height);
void graphics_destroy(Graphics* graphics);
void graphics_set_pixel(const Graphics* graphics, int x, int y, Color color);
void graphics_clear(const Graphics* graphics, Color color);
void graphics_draw_line(const Graphics* graphics, const Vec3* p0, const Vec3* p1, Color color);
void graphics_draw_triangle(const Graphics* graphics, const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color);

#endif
