#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "vector.h"

#include <stdint.h>

typedef uint32_t Color;

extern const int g_screen_width;
extern const int g_screen_height;
extern Color* g_pixel_buffer;

void initialize_graphics(void);
void deinitialize_graphics(void);
void put_pixel(int x, int y, Color color);
void clear(Color color);
void draw_triangle(const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color);

#endif
