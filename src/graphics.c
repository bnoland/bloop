#include "graphics.h"

#include <assert.h>
#include <stdlib.h>
#include <tgmath.h>

static void
graphics_draw_triangle_flat_bottom(Graphics* graphics, const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color);
static void
graphics_draw_triangle_flat_top(Graphics* graphics, const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color);

static void swap(const Vec3** v, const Vec3** w);

void graphics_init(Graphics* graphics, int screen_width, int screen_height)
{
  graphics->screen_width = screen_width;
  graphics->screen_height = screen_height;
  graphics->pixel_buffer = (Color*)malloc(screen_width * screen_height * sizeof(Color));
}

void graphics_destroy(Graphics* graphics)
{
  free(graphics->pixel_buffer);
  graphics->pixel_buffer = NULL;
}

void graphics_set_pixel(Graphics* graphics, int x, int y, Color color)
{
  // XXX: Temporary. Replace with assertions later.
  if (x < 0 || x >= graphics->screen_width) return;
  if (y < 0 || y >= graphics->screen_height) return;

  // assert(x >= 0 && x < graphics->screen_width);
  // assert(y >= 0 && y < graphics->screen_height);

  graphics->pixel_buffer[x + y * graphics->screen_width] = color;
}

void graphics_clear(Graphics* graphics, Color color)
{
  const int screen_width = graphics->screen_width;
  const int screen_height = graphics->screen_height;

  for (int y = 0; y < screen_height; y++) {
    for (int x = 0; x < screen_width; x++) {
      graphics_set_pixel(graphics, x, y, color);
    }
  }
}

void graphics_draw_triangle(Graphics* graphics, const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color)
{
  if (p0->y > p1->y) swap(&p0, &p1);
  if (p0->y > p2->y) swap(&p0, &p2);
  if (p1->y > p2->y) swap(&p1, &p2);

  Vec3 q;
  vec3_interpolate(&q, p0, p2, (p1->y - p0->y) / (p2->y - p0->y));

  if (q.x > p1->x) {
    graphics_draw_triangle_flat_bottom(graphics, p0, p1, &q, color);
    graphics_draw_triangle_flat_top(graphics, p1, &q, p2, color);
  } else {
    graphics_draw_triangle_flat_bottom(graphics, p0, &q, p1, color);
    graphics_draw_triangle_flat_top(graphics, &q, p1, p2, color);
  }
}

// `p1` and `p2` form the flat bottom of the triangle.
static void
graphics_draw_triangle_flat_bottom(Graphics* graphics, const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color)
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
      graphics_set_pixel(graphics, x, y, color);
    }

    vec3_add(&left, &left, &left_inc);
    vec3_add(&right, &right, &right_inc);
  }
}

// `p0` and `p1` form the flat top of the triangle.
static void
graphics_draw_triangle_flat_top(Graphics* graphics, const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color)
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
      graphics_set_pixel(graphics, x, y, color);
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
