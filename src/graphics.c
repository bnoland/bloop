#include "graphics.h"

#include <stdlib.h>
#include <assert.h>
#include <tgmath.h>

const int g_screen_width = 800;
const int g_screen_height = 600;
Color* g_pixel_buffer;

static void draw_triangle_flat_bottom(const Vec3* v0, const Vec3* v1, const Vec3* v2, Color color);
static void draw_triangle_flat_top(const Vec3* v0, const Vec3* v1, const Vec3* v2, Color color);
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

void draw_triangle(const Vec3* v0, const Vec3* v1, const Vec3* v2, Color color)
{
  if (v0->y > v1->y) swap(&v0, &v1);
  if (v0->y > v2->y) swap(&v0, &v2);
  if (v1->y > v2->y) swap(&v1, &v2);

  Vec3 q;
  vec3_interpolate(&q, v0, v2, (v1->y - v0->y) / (v2->y - v0->y));

  if (q.x > v1->x) {
    draw_triangle_flat_bottom(v0, v1, &q, color);
    draw_triangle_flat_top(v1, &q, v2, color);
  } else {
    draw_triangle_flat_bottom(v0, &q, v1, color);
    draw_triangle_flat_top(&q, v1, v2, color);
  }
}

// `v1` and `v2` form the flat bottom of the triangle.
static void draw_triangle_flat_bottom(const Vec3* v0, const Vec3* v1, const Vec3* v2, Color color)
{
  const float height = v1->y - v0->y;
  const int y_start = ceil(v0->y - 0.5f);
  const int y_end = ceil(v1->y - 0.5f);

  // (v1 - v0) / height
  Vec3 left_inc;
  vec3_sub(&left_inc, v1, v0);
  vec3_mul(&left_inc, &left_inc, 1.0f / height);

  // (v2 - v0) / height
  Vec3 right_inc;
  vec3_sub(&right_inc, v2, v0);
  vec3_mul(&right_inc, &right_inc, 1.0f / height);

  Vec3 left;
  vec3_mul_add(&left, v0, &left_inc, y_start + 0.5f - v0->y);  // pre-step

  Vec3 right;
  vec3_mul_add(&right, v0, &right_inc, y_start + 0.5f - v0->y);  // pre-step

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

// `v0` and `v1` form the flat top of the triangle.
static void draw_triangle_flat_top(const Vec3* v0, const Vec3* v1, const Vec3* v2, Color color)
{
  const float height = v2->y - v0->y;
  const int y_start = ceil(v0->y - 0.5f);
  const int y_end = ceil(v2->y - 0.5f);

  // (v2 - v0) / height
  Vec3 left_inc;
  vec3_sub(&left_inc, v2, v0);
  vec3_mul(&left_inc, &left_inc, 1.0f / height);

  // (v2 - v1) / height
  Vec3 right_inc;
  vec3_sub(&right_inc, v2, v1);
  vec3_mul(&right_inc, &right_inc, 1.0f / height);

  Vec3 left;
  vec3_mul_add(&left, v0, &left_inc, y_start + 0.5f - v0->y);  // pre-step

  Vec3 right;
  vec3_mul_add(&right, v1, &right_inc, y_start + 0.5f - v1->y);  // pre-step

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
