#ifndef PIPELINE_IMPLEMENTATION

#include "graphics.h"

#include <tgmath.h>

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
  EFFECT effect;
} PIPELINE;

PIPELINE PIPELINE_PREFIX(pipeline_make)(const Graphics* graphics);
void PIPELINE_PREFIX(pipeline_draw)(const PIPELINE* pipeline, const MESH* mesh);

#else

#include <stddef.h>

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

static void swap(const GS_OUT** v, const GS_OUT** w);

PIPELINE PIPELINE_PREFIX(pipeline_make)(const Graphics* graphics)
{
  return (PIPELINE){
    .graphics = graphics,
    .effect = EFFECT_MAKE(),
  };
}

void PIPELINE_PREFIX(pipeline_draw)(const PIPELINE* pipeline, const MESH* mesh)
{
  DynList trans_verts = dyn_list_make(sizeof(VS_OUT));

  for (size_t i = 0; i < mesh->vertices.size; i++) {
    const VERTEX* v = dyn_list_at(&mesh->vertices, i);
    VS_OUT* vs = dyn_list_add_slot(&trans_verts);
    VERTEX_SHADER(&pipeline->effect, v, vs);
  }

  for (size_t i = 0; i < mesh->indices.size / 3; i++) {
    const size_t* i0 = dyn_list_at(&mesh->indices, 3 * i);
    const size_t* i1 = dyn_list_at(&mesh->indices, 3 * i + 1);
    const size_t* i2 = dyn_list_at(&mesh->indices, 3 * i + 2);

    const VS_OUT* vs0 = dyn_list_at(&trans_verts, *i0);
    const VS_OUT* vs1 = dyn_list_at(&trans_verts, *i1);
    const VS_OUT* vs2 = dyn_list_at(&trans_verts, *i2);

    GS_OUT gs0;
    GS_OUT gs1;
    GS_OUT gs2;
    GEOMETRY_SHADER(&pipeline->effect, vs0, vs1, vs2, &gs0, &gs1, &gs2, i);

    pipeline_transform_to_screen(pipeline, &gs0);
    pipeline_transform_to_screen(pipeline, &gs1);
    pipeline_transform_to_screen(pipeline, &gs2);

    pipeline_draw_triangle(pipeline, &gs0, &gs1, &gs2);
  }

  dyn_list_destroy(&trans_verts);
}

static void pipeline_transform_to_screen(const PIPELINE* pipeline, GS_OUT* v)
{
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
  const int y_start = ceil(left_start->pos.y - 0.5f);
  const int y_end = ceil(left_start->pos.y + height - 0.5f);

  GS_OUT left = GS_OUT_MUL_ADD(left_start, left_inc, y_start + 0.5f - left_start->pos.y);
  GS_OUT right = GS_OUT_MUL_ADD(right_start, right_inc, y_start + 0.5f - right_start->pos.y);

  for (int y = y_start; y < y_end; y++) {
    const int x_start = ceil(left.pos.x - 0.5f);
    const int x_end = ceil(right.pos.x - 0.5f);

    const float width = right.pos.x - left.pos.x;

    for (int x = x_start; x < x_end; x++) {
      const GS_OUT scan = GS_OUT_INTERPOLATE(&left, &right, (x + 0.5f - left.pos.x) / width);
      const Color color = PIXEL_SHADER(&pipeline->effect, &scan);
      graphics_set_pixel(pipeline->graphics, x, y, color);
    }

    left = GS_OUT_ADD(&left, left_inc);
    right = GS_OUT_ADD(&right, right_inc);
  }
}

static void swap(const GS_OUT** v, const GS_OUT** w)
{
  const GS_OUT* temp = *v;
  *v = *w;
  *w = temp;
}

#endif
