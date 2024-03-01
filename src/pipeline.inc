#ifndef PIPELINE_IMPLEMENTATION

#include "graphics.h"
#include "depth_buffer.h"
#include "meshes/default_mesh.h"

#include <tgmath.h>

#ifndef PIPELINE_FUNCTION_PREFIX
#define PIPELINE_FUNCTION_PREFIX Default
#endif
#ifndef PIPELINE_TYPE_PREFIX
#define PIPELINE_TYPE_PREFIX default_
#endif
#ifndef PIPELINE_MESH_TYPE
#define PIPELINE_MESH_TYPE DefaultMesh
#endif
#ifndef PIPELINE_EFFECT_TYPE
#define PIPELINE_EFFECT_TYPE DefaultEffect
#endif
#ifndef PIPELINE_EFFECT_FUNCTION_PREFIX
#define PIPELINE_EFFECT_FUNCTION_PREFIX default_effect_
#endif

#undef _CONCAT
#undef CONCAT
#undef PIPELINE_PREFIX
#undef PIPELINE
#undef MESH
#undef EFFECT
#undef VERTEX
#undef VS_OUT
#undef GS_OUT
#undef GS_OUT_ADD
#undef GS_OUT_SUB
#undef GS_OUT_MUL
#undef GS_OUT_MUL_ADD
#undef GS_OUT_INTERPOLATE
#undef EFFECT_MAKE
#undef VERTEX_SHADER
#undef GEOMETRY_SHADER
#undef PIXEL_SHADER

#define _CONCAT(x, y)         x##y
#define CONCAT(x, y)          _CONCAT(x, y)
#define PIPELINE_PREFIX(name) CONCAT(PIPELINE_FUNCTION_PREFIX, name)
#define PIPELINE              CONCAT(PIPELINE_TYPE_PREFIX, Pipeline)
#define MESH                  PIPELINE_MESH_TYPE
#define EFFECT                PIPELINE_EFFECT_TYPE
#define VERTEX                CONCAT(PIPELINE_EFFECT_TYPE, Vertex)
#define VS_OUT                CONCAT(PIPELINE_EFFECT_TYPE, VSOut)
#define GS_OUT                CONCAT(PIPELINE_EFFECT_TYPE, GSOut)
#define GS_OUT_ADD            CONCAT(PIPELINE_EFFECT_FUNCTION_PREFIX, gsout_add)
#define GS_OUT_SUB            CONCAT(PIPELINE_EFFECT_FUNCTION_PREFIX, gsout_sub)
#define GS_OUT_MUL            CONCAT(PIPELINE_EFFECT_FUNCTION_PREFIX, gsout_mul)
#define GS_OUT_MUL_ADD        CONCAT(PIPELINE_EFFECT_FUNCTION_PREFIX, gsout_mul_add)
#define GS_OUT_INTERPOLATE    CONCAT(PIPELINE_EFFECT_FUNCTION_PREFIX, gsout_interpolate)
#define EFFECT_MAKE           CONCAT(PIPELINE_EFFECT_FUNCTION_PREFIX, make)
#define VERTEX_SHADER         CONCAT(PIPELINE_EFFECT_FUNCTION_PREFIX, vertex_shader)
#define GEOMETRY_SHADER       CONCAT(PIPELINE_EFFECT_FUNCTION_PREFIX, geometry_shader)
#define PIXEL_SHADER          CONCAT(PIPELINE_EFFECT_FUNCTION_PREFIX, pixel_shader)

typedef struct
{
  const Graphics* graphics;
  const DepthBuffer* depth_buffer;
  EFFECT effect;
} PIPELINE;

PIPELINE PIPELINE_PREFIX(pipeline_make)(const Graphics* graphics, const DepthBuffer* depth_buffer);
void PIPELINE_PREFIX(pipeline_draw)(const PIPELINE* pipeline, const MESH* mesh);

#else

#include <stddef.h>

static void pipeline_process_vertices(const PIPELINE* pipeline, const DynList* vertices, const DynList* indices);
static void pipeline_assemble_triangles(const PIPELINE* pipeline, const DynList* vertices, const DynList* indices);
static void pipeline_process_triangle(const PIPELINE* pipeline,
                                      const VS_OUT* v0,
                                      const VS_OUT* v1,
                                      const VS_OUT* v2,
                                      size_t triangle_index);
static void pipeline_post_process_triangle(const PIPELINE* pipeline, GS_OUT* v0, GS_OUT* v1, GS_OUT* v2);

static void pipeline_clip_triangle(const PIPELINE* pipeline, GS_OUT* v0, GS_OUT* v1, GS_OUT* v2);
static void pipeline_clip_triangle1(const PIPELINE* pipeline, GS_OUT* v0, GS_OUT* v1, GS_OUT* v2);
static void pipeline_clip_triangle2(const PIPELINE* pipeline, GS_OUT* v0, GS_OUT* v1, GS_OUT* v2);
static void pipeline_transform_to_screen(const PIPELINE* pipeline, GS_OUT* v);

static void pipeline_draw_triangle(const PIPELINE* pipeline, const GS_OUT* v0, const GS_OUT* v1, const GS_OUT* v2);
static void pipeline_draw_triangle_flat_bottom(const PIPELINE* pipeline,
                                               const GS_OUT* v0,
                                               const GS_OUT* v1,
                                               const GS_OUT* v2);
static void pipeline_draw_triangle_flat_top(const PIPELINE* pipeline,
                                            const GS_OUT* v0,
                                            const GS_OUT* v1,
                                            const GS_OUT* v2);
static void pipeline_draw_triangle_flat(const PIPELINE* pipeline,
                                        const GS_OUT* left_start,
                                        const GS_OUT* right_start,
                                        const GS_OUT* left_inc,
                                        const GS_OUT* right_inc,
                                        float height);

static bool triangle_visible(const GS_OUT* v0, const GS_OUT* v1, const GS_OUT* v2);
static void swap(const GS_OUT** v, const GS_OUT** w);

PIPELINE PIPELINE_PREFIX(pipeline_make)(const Graphics* graphics, const DepthBuffer* depth_buffer)
{
  return (PIPELINE){
    .graphics = graphics,
    .depth_buffer = depth_buffer,
    .effect = EFFECT_MAKE(),
  };
}

void PIPELINE_PREFIX(pipeline_draw)(const PIPELINE* pipeline, const MESH* mesh)
{
  depth_buffer_clear(pipeline->depth_buffer);
  pipeline_process_vertices(pipeline, &mesh->vertices, &mesh->indices);
}

static void pipeline_process_vertices(const PIPELINE* pipeline, const DynList* vertices, const DynList* indices)
{
  DynList trans_verts = dyn_list_make(sizeof(VS_OUT));

  for (size_t i = 0; i < vertices->size; i++) {
    const VERTEX* v = dyn_list_at(vertices, i);
    VS_OUT* vs = dyn_list_add_slot(&trans_verts);
    VERTEX_SHADER(&pipeline->effect, v, vs);
  }

  pipeline_assemble_triangles(pipeline, &trans_verts, indices);

  dyn_list_destroy(&trans_verts);
}

static void pipeline_assemble_triangles(const PIPELINE* pipeline, const DynList* vertices, const DynList* indices)
{
  for (size_t i = 0; i < indices->size / 3; i++) {
    const size_t* i0 = dyn_list_at(indices, 3 * i);
    const size_t* i1 = dyn_list_at(indices, 3 * i + 1);
    const size_t* i2 = dyn_list_at(indices, 3 * i + 2);

    const VS_OUT* v0 = dyn_list_at(vertices, *i0);
    const VS_OUT* v1 = dyn_list_at(vertices, *i1);
    const VS_OUT* v2 = dyn_list_at(vertices, *i2);

    pipeline_process_triangle(pipeline, v0, v1, v2, i);
  }
}

static void pipeline_process_triangle(const PIPELINE* pipeline,
                                      const VS_OUT* v0,
                                      const VS_OUT* v1,
                                      const VS_OUT* v2,
                                      size_t triangle_index)
{
  GS_OUT w0;
  GS_OUT w1;
  GS_OUT w2;
  GEOMETRY_SHADER(&pipeline->effect, v0, v1, v2, &w0, &w1, &w2, triangle_index);

  pipeline_clip_triangle(pipeline, &w0, &w1, &w2);
}

static void pipeline_post_process_triangle(const PIPELINE* pipeline, GS_OUT* v0, GS_OUT* v1, GS_OUT* v2)
{
  pipeline_transform_to_screen(pipeline, v0);
  pipeline_transform_to_screen(pipeline, v1);
  pipeline_transform_to_screen(pipeline, v2);

  if (triangle_visible(v0, v1, v2)) {
    pipeline_draw_triangle(pipeline, v0, v1, v2);
  }
}

static void pipeline_clip_triangle(const PIPELINE* pipeline, GS_OUT* v0, GS_OUT* v1, GS_OUT* v2)
{
  // Is the triangle completely off screen?

  if (v0->pos.x > v0->pos.w && v1->pos.x > v1->pos.w && v2->pos.x > v2->pos.w) return;
  if (v0->pos.x < -v0->pos.w && v1->pos.x < -v1->pos.w && v2->pos.x < -v2->pos.w) return;

  if (v0->pos.y > v0->pos.w && v1->pos.y > v1->pos.w && v2->pos.y > v2->pos.w) return;
  if (v0->pos.y < -v0->pos.w && v1->pos.y < -v1->pos.w && v2->pos.y < -v2->pos.w) return;

  if (v0->pos.z > v0->pos.w && v1->pos.z > v1->pos.w && v2->pos.z > v2->pos.w) return;
  if (v0->pos.z < -v0->pos.w && v1->pos.z < -v1->pos.w && v2->pos.z < -v2->pos.w) return;

  // Is part of the triangle off screen?

  if (v0->pos.z < -v0->pos.w && v1->pos.z < -v1->pos.w) {
    pipeline_clip_triangle2(pipeline, v0, v1, v2);
    return;
  }
  if (v0->pos.z < -v0->pos.w && v2->pos.z < -v2->pos.w) {
    pipeline_clip_triangle2(pipeline, v0, v2, v1);
    return;
  }
  if (v1->pos.z < -v1->pos.w && v2->pos.z < -v2->pos.w) {
    pipeline_clip_triangle2(pipeline, v1, v2, v0);
    return;
  }

  if (v0->pos.z < -v0->pos.w) {
    pipeline_clip_triangle1(pipeline, v0, v1, v2);
    return;
  }
  if (v1->pos.z < -v1->pos.w) {
    pipeline_clip_triangle1(pipeline, v1, v0, v2);
    return;
  }
  if (v2->pos.z < -v2->pos.w) {
    pipeline_clip_triangle1(pipeline, v2, v0, v1);
    return;
  }

  // All of the triangle is on screen.

  pipeline_post_process_triangle(pipeline, v0, v1, v2);
}

// `v0` is off screen.
static void pipeline_clip_triangle1(const PIPELINE* pipeline, GS_OUT* v0, GS_OUT* v1, GS_OUT* v2)
{
  const float alpha0 = (-v1->pos.w - v1->pos.z) / (v0->pos.z + v0->pos.w - v1->pos.w - v1->pos.z);
  const float alpha1 = (-v2->pos.w - v2->pos.z) / (v0->pos.z + v0->pos.w - v2->pos.w - v2->pos.z);

  GS_OUT u = GS_OUT_INTERPOLATE(v0, v1, alpha0);
  GS_OUT w = GS_OUT_INTERPOLATE(v0, v2, alpha1);
  GS_OUT temp1 = u;  // XXX: Eliminate the need for this.
  GS_OUT temp2 = *v2;  // XXX: Eliminate the need for this.
  pipeline_post_process_triangle(pipeline, &temp1, &temp2, v1);
  pipeline_post_process_triangle(pipeline, &w, v2, &u);
}

// `v0` and `v1` are off screen.
static void pipeline_clip_triangle2(const PIPELINE* pipeline, GS_OUT* v0, GS_OUT* v1, GS_OUT* v2)
{
  const float alpha0 = (-v2->pos.w - v2->pos.z) / (v0->pos.z + v0->pos.w - v2->pos.w - v2->pos.z);
  const float alpha1 = (-v2->pos.w - v2->pos.z) / (v1->pos.z + v1->pos.w - v2->pos.w - v2->pos.z);

  GS_OUT u = GS_OUT_INTERPOLATE(v0, v2, alpha0);
  GS_OUT w = GS_OUT_INTERPOLATE(v1, v2, alpha1);
  pipeline_post_process_triangle(pipeline, &u, &w, v2);
}

static void pipeline_transform_to_screen(const PIPELINE* pipeline, GS_OUT* v)
{
  // Perspective divide.
  v->pos.x /= v->pos.w;
  v->pos.y /= v->pos.w;
  v->pos.z /= v->pos.w;

  const size_t screen_width = pipeline->graphics->screen_width;
  const size_t screen_height = pipeline->graphics->screen_height;

  v->pos.x = (screen_width / 2.0f) * (v->pos.x + 1.0f);
  v->pos.y = (screen_height / 2.0f) * (-v->pos.y + 1.0f);
}

static void pipeline_draw_triangle(const PIPELINE* pipeline, const GS_OUT* v0, const GS_OUT* v1, const GS_OUT* v2)
{
  if (v0->pos.y > v1->pos.y) swap(&v0, &v1);
  if (v0->pos.y > v2->pos.y) swap(&v0, &v2);
  if (v1->pos.y > v2->pos.y) swap(&v1, &v2);

  const GS_OUT q = GS_OUT_INTERPOLATE(v0, v2, (v1->pos.y - v0->pos.y) / (v2->pos.y - v0->pos.y));

  if (q.pos.x > v1->pos.x) {
    pipeline_draw_triangle_flat_bottom(pipeline, v0, v1, &q);
    pipeline_draw_triangle_flat_top(pipeline, v1, &q, v2);
  } else {
    pipeline_draw_triangle_flat_bottom(pipeline, v0, &q, v1);
    pipeline_draw_triangle_flat_top(pipeline, &q, v1, v2);
  }
}

// `v1` and `v2` form the flat bottom of the triangle.
static void pipeline_draw_triangle_flat_bottom(const PIPELINE* pipeline,
                                               const GS_OUT* v0,
                                               const GS_OUT* v1,
                                               const GS_OUT* v2)
{
  const float height = v1->pos.y - v0->pos.y;

  // (v1 - v0) / height
  GS_OUT left_inc = GS_OUT_SUB(v1, v0);
  left_inc = GS_OUT_MUL(&left_inc, 1.0f / height);

  // (v2 - v0) / height
  GS_OUT right_inc = GS_OUT_SUB(v2, v0);
  right_inc = GS_OUT_MUL(&right_inc, 1.0f / height);

  pipeline_draw_triangle_flat(pipeline, v0, v0, &left_inc, &right_inc, height);
}

// `v0` and `v1` form the flat top of the triangle.
static void pipeline_draw_triangle_flat_top(const PIPELINE* pipeline,
                                            const GS_OUT* v0,
                                            const GS_OUT* v1,
                                            const GS_OUT* v2)
{
  const float height = v2->pos.y - v0->pos.y;

  // (v2 - v0) / height
  GS_OUT left_inc = GS_OUT_SUB(v2, v0);
  left_inc = GS_OUT_MUL(&left_inc, 1.0f / height);

  // (v2 - v1) / height
  GS_OUT right_inc = GS_OUT_SUB(v2, v1);
  right_inc = GS_OUT_MUL(&right_inc, 1.0f / height);

  pipeline_draw_triangle_flat(pipeline, v0, v1, &left_inc, &right_inc, height);
}

static void pipeline_draw_triangle_flat(const PIPELINE* pipeline,
                                        const GS_OUT* left_start,
                                        const GS_OUT* right_start,
                                        const GS_OUT* left_inc,
                                        const GS_OUT* right_inc,
                                        float height)
{
  const int screen_width = pipeline->graphics->screen_width;
  const int screen_height = pipeline->graphics->screen_height;

  const int y_start = fmax(ceil(left_start->pos.y - 0.5f), 0);
  const int y_end = fmin(ceil(left_start->pos.y + height - 0.5f), screen_height - 1);

  GS_OUT left = GS_OUT_MUL_ADD(left_start, left_inc, y_start + 0.5f - left_start->pos.y);
  GS_OUT right = GS_OUT_MUL_ADD(right_start, right_inc, y_start + 0.5f - right_start->pos.y);

  for (int y = y_start; y < y_end; y++) {
    const int x_start = fmax(ceil(left.pos.x - 0.5f), 0);
    const int x_end = fmin(ceil(right.pos.x - 0.5f), screen_width - 1);

    const float width = right.pos.x - left.pos.x;

    for (int x = x_start; x < x_end; x++) {
      const GS_OUT scan = GS_OUT_INTERPOLATE(&left, &right, (x + 0.5f - left.pos.x) / width);
      if (depth_buffer_test_and_set(pipeline->depth_buffer, x, y, scan.pos.z)) {
        const Color color = PIXEL_SHADER(&pipeline->effect, &scan);
        graphics_set_pixel(pipeline->graphics, x, y, color);
      }
    }

    left = GS_OUT_ADD(&left, left_inc);
    right = GS_OUT_ADD(&right, right_inc);
  }
}

static bool triangle_visible(const GS_OUT* v0, const GS_OUT* v1, const GS_OUT* v2)
{
  // A triangle is visible iff its vertices have a counter-clockwise winding in screen space.
  const Vec4 v = vec4_sub(&v1->pos, &v0->pos);
  const Vec4 w = vec4_sub(&v2->pos, &v0->pos);
  return v.x * w.y - v.y * w.x < 0;
}

static void swap(const GS_OUT** v, const GS_OUT** w)
{
  const GS_OUT* temp = *v;
  *v = *w;
  *w = temp;
}

#endif
