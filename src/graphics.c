#include "graphics.h"

#include <stdlib.h>
#include <assert.h>
#include <tgmath.h>

const int g_screen_width = 800;
const int g_screen_height = 600;
Color* g_pixel_buffer;

static void draw_triangle_flat_bottom(const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color);
static void draw_triangle_flat_top(const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color);
static void swap(const Vec3** v, const Vec3** w);

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
  // XXX: Temporary. Replace with assertions later.
  if (x < 0 || x >= g_screen_width) return;
  if (y < 0 || y >= g_screen_height) return;

  // assert(x >= 0 && x < g_screen_width);
  // assert(y >= 0 && y < g_screen_height);

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

void draw_triangle(const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color)
{
  if (p0->y > p1->y) swap(&p0, &p1);
  if (p0->y > p2->y) swap(&p0, &p2);
  if (p1->y > p2->y) swap(&p1, &p2);

  Vec3 q;
  vec3_interpolate(&q, p0, p2, (p1->y - p0->y) / (p2->y - p0->y));

  if (q.x > p1->x) {
    draw_triangle_flat_bottom(p0, p1, &q, color);
    draw_triangle_flat_top(p1, &q, p2, color);
  } else {
    draw_triangle_flat_bottom(p0, &q, p1, color);
    draw_triangle_flat_top(&q, p1, p2, color);
  }
}

// `p1` and `p2` form the flat bottom of the triangle.
static void draw_triangle_flat_bottom(const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color)
{
  const float height = p1->y - p0->y;
  const int y_start = ceil(p0->y - 0.5f);
  const int y_end = ceil(p1->y - 0.5f);

  // (p1 - p0) / height
  Vec3 left_inc;
  vec3_sub(&left_inc, p1, p0);
  vec3_mul(&left_inc, &left_inc, 1.0f / height);

  // (p2 - p0) / height
  Vec3 right_inc;
  vec3_sub(&right_inc, p2, p0);
  vec3_mul(&right_inc, &right_inc, 1.0f / height);

  Vec3 left;
  vec3_mul_add(&left, p0, &left_inc, y_start + 0.5f - p0->y);  // pre-step

  Vec3 right;
  vec3_mul_add(&right, p0, &right_inc, y_start + 0.5f - p0->y);  // pre-step

  for (int y = y_start; y < y_end; y++) {
    const int x_start = ceil(left.x - 0.5f);
    const int x_end = ceil(right.x - 0.5f);

    for (int x = x_start; x < x_end; x++) {
      put_pixel(x, y, color);
    }

    vec3_add(&left, &left, &left_inc);
    vec3_add(&right, &right, &right_inc);
  }
}

// `p0` and `p1` form the flat top of the triangle.
static void draw_triangle_flat_top(const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color)
{
  const float height = p2->y - p0->y;
  const int y_start = ceil(p0->y - 0.5f);
  const int y_end = ceil(p2->y - 0.5f);

  // (p2 - p0) / height
  Vec3 left_inc;
  vec3_sub(&left_inc, p2, p0);
  vec3_mul(&left_inc, &left_inc, 1.0f / height);

  // (p2 - p1) / height
  Vec3 right_inc;
  vec3_sub(&right_inc, p2, p1);
  vec3_mul(&right_inc, &right_inc, 1.0f / height);

  Vec3 left;
  vec3_mul_add(&left, p0, &left_inc, y_start + 0.5f - p0->y);  // pre-step

  Vec3 right;
  vec3_mul_add(&right, p1, &right_inc, y_start + 0.5f - p1->y);  // pre-step

  for (int y = y_start; y < y_end; y++) {
    const int x_start = ceil(left.x - 0.5f);
    const int x_end = ceil(right.x - 0.5f);

    for (int x = x_start; x < x_end; x++) {
      put_pixel(x, y, color);
    }

    vec3_add(&left, &left, &left_inc);
    vec3_add(&right, &right, &right_inc);
  }
}

static void swap(const Vec3** v, const Vec3** w)
{
  const Vec3* temp = *v;
  *v = *w;
  *w = temp;
}
