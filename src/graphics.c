#include "graphics.h"

#include <assert.h>
#include <stdlib.h>
#include <tgmath.h>
#include <stdbool.h>

static void graphics_draw_triangle_flat_bottom(const Graphics* graphics,
                                               const Vec3* p0,
                                               const Vec3* p1,
                                               const Vec3* p2,
                                               Color color);
static void graphics_draw_triangle_flat_top(const Graphics* graphics,
                                            const Vec3* p0,
                                            const Vec3* p1,
                                            const Vec3* p2,
                                            Color color);
static void graphics_draw_triangle_flat(const Graphics* graphics,
                                        const Vec3* left_start,
                                        const Vec3* right_start,
                                        const Vec3* left_inc,
                                        const Vec3* right_inc,
                                        float height,
                                        Color color);

static void swap_vec_ptrs(const Vec3** v, const Vec3** w);
static void swap_ints(int* x, int* y);

Graphics graphics_make(int screen_width, int screen_height)
{
  return (Graphics){
    .screen_width = screen_width,
    .screen_height = screen_height,
    .pixel_buffer = malloc(screen_width * screen_height * sizeof(Color)),
  };
}

void graphics_destroy(Graphics* graphics)
{
  free(graphics->pixel_buffer);
  graphics->pixel_buffer = NULL;
}

void graphics_set_pixel(const Graphics* graphics, int x, int y, Color color)
{
  assert(x >= 0 && x < graphics->screen_width);
  assert(y >= 0 && y < graphics->screen_height);
  assert(graphics->pixel_buffer != NULL);

  graphics->pixel_buffer[x + y * graphics->screen_width] = color;
}

void graphics_clear(const Graphics* graphics, Color color)
{
  const int screen_width = graphics->screen_width;
  const int screen_height = graphics->screen_height;

  for (int y = 0; y < screen_height; y++) {
    for (int x = 0; x < screen_width; x++) {
      graphics_set_pixel(graphics, x, y, color);
    }
  }
}

void graphics_draw_line(const Graphics* graphics, const Vec3* p0, const Vec3* p1, Color color)
{
  int x0 = p0->x;
  int y0 = p0->y;
  int x1 = p1->x;
  int y1 = p1->y;

  bool steep = false;
  if (abs(y1 - y0) > abs(x1 - x0)) {
    swap_ints(&x0, &y0);
    swap_ints(&x1, &y1);
    steep = true;
  }
  if (x1 < x0) {
    swap_ints(&x0, &x1);
    swap_ints(&y0, &y1);
  }

  const int dx = x1 - x0;
  const int dy = y1 - y0;
  const int derror = 2 * abs(dy);
  const int y_inc = y1 > y0 ? 1 : -1;
  int error = 0;
  int y = y0;

  if (steep) {
    for (int x = x0; x <= x1; x++) {
      graphics_set_pixel(graphics, y, x, color);
      error += derror;
      if (error >= dx) {
        y += y_inc;
        error -= 2 * dx;
      }
    }
  } else {
    for (int x = x0; x <= x1; x++) {
      graphics_set_pixel(graphics, x, y, color);
      error += derror;
      if (error >= dx) {
        y += y_inc;
        error -= 2 * dx;
      }
    }
  }
}

void graphics_draw_triangle(const Graphics* graphics, const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color)
{
  if (p0->y > p1->y) swap_vec_ptrs(&p0, &p1);
  if (p0->y > p2->y) swap_vec_ptrs(&p0, &p2);
  if (p1->y > p2->y) swap_vec_ptrs(&p1, &p2);

  const Vec3 q = vec3_interpolate(p0, p2, (p1->y - p0->y) / (p2->y - p0->y));

  if (q.x > p1->x) {
    graphics_draw_triangle_flat_bottom(graphics, p0, p1, &q, color);
    graphics_draw_triangle_flat_top(graphics, p1, &q, p2, color);
  } else {
    graphics_draw_triangle_flat_bottom(graphics, p0, &q, p1, color);
    graphics_draw_triangle_flat_top(graphics, &q, p1, p2, color);
  }
}

// `p1` and `p2` form the flat bottom of the triangle.
static void graphics_draw_triangle_flat_bottom(const Graphics* graphics,
                                               const Vec3* p0,
                                               const Vec3* p1,
                                               const Vec3* p2,
                                               Color color)
{
  const float height = p1->y - p0->y;

  // (p1 - p0) / height
  Vec3 left_inc = vec3_sub(p1, p0);
  left_inc = vec3_mul(&left_inc, 1.0f / height);

  // (p2 - p0) / height
  Vec3 right_inc = vec3_sub(p2, p0);
  right_inc = vec3_mul(&right_inc, 1.0f / height);

  graphics_draw_triangle_flat(graphics, p0, p0, &left_inc, &right_inc, height, color);
}

// `p0` and `p1` form the flat top of the triangle.
static void graphics_draw_triangle_flat_top(const Graphics* graphics,
                                            const Vec3* p0,
                                            const Vec3* p1,
                                            const Vec3* p2,
                                            Color color)
{
  const float height = p2->y - p0->y;

  // (p2 - p0) / height
  Vec3 left_inc = vec3_sub(p2, p0);
  left_inc = vec3_mul(&left_inc, 1.0f / height);

  // (p2 - p1) / height
  Vec3 right_inc = vec3_sub(p2, p1);
  right_inc = vec3_mul(&right_inc, 1.0f / height);

  graphics_draw_triangle_flat(graphics, p0, p1, &left_inc, &right_inc, height, color);
}

static void graphics_draw_triangle_flat(const Graphics* graphics,
                                        const Vec3* left_start,
                                        const Vec3* right_start,
                                        const Vec3* left_inc,
                                        const Vec3* right_inc,
                                        float height,
                                        Color color)
{
  const int y_start = ceil(left_start->y - 0.5f);
  const int y_end = ceil(left_start->y + height - 0.5f);

  Vec3 left = vec3_mul_add(left_start, left_inc, y_start + 0.5f - left_start->y);
  Vec3 right = vec3_mul_add(right_start, right_inc, y_start + 0.5f - right_start->y);

  for (int y = y_start; y < y_end; y++) {
    const int x_start = ceil(left.x - 0.5f);
    const int x_end = ceil(right.x - 0.5f);

    for (int x = x_start; x < x_end; x++) {
      graphics_set_pixel(graphics, x, y, color);
    }

    left = vec3_add(&left, left_inc);
    right = vec3_add(&right, right_inc);
  }
}

static void swap_vec_ptrs(const Vec3** v, const Vec3** w)
{
  const Vec3* temp = *v;
  *v = *w;
  *w = temp;
}

static void swap_ints(int* x, int* y)
{
  const int temp = *x;
  *x = *y;
  *y = temp;
}
