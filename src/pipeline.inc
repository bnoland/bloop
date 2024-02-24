
// Bail if we're missing any of the required parameters.
#ifndef PIPELINE_TYPE_PREFIX
#error "PIPELINE_TYPE_PREFIX is undefined"
#endif
#ifndef PIPELINE_FUNCTION_PREFIX
#error "PIPELINE_FUNCTION_PREFIX is undefined"
#endif
#ifndef PIPELINE_VERTEX_TYPE
#error "PIPELINE_VERTEX_TYPE is undefined"
#endif
#ifndef PIPELINE_VS_OUT_TYPE
#error "PIPELINE_VS_OUT_TYPE is undefined"
#endif
#ifndef PIPELINE_GS_OUT_TYPE
#error "PIPELINE_GS_OUT_TYPE is undefined"
#endif
#ifndef GS_OUT_ADD
#error "GS_OUT_ADD is undefined"
#endif
#ifndef GS_OUT_SUB
#error "GS_OUT_SUB is undefined"
#endif
#ifndef GS_OUT_MUL
#error "GS_OUT_MUL is undefined"
#endif
#ifndef GS_OUT_MUL_ADD
#error "GS_OUT_MUL_ADD is undefined"
#endif
#ifndef GS_OUT_INTERPOLATE
#error "GS_OUT_INTERPOLATE is undefined"
#endif
#ifndef PIPELINE_MESH_TYPE
#error "PIPELINE_MESH_TYPE is undefined"
#endif

#ifndef PIPELINE_IMPLEMENTATION

#include "graphics.h"

#include <stddef.h>
#include <tgmath.h>

#define _CONCAT(x, y) x##y
#define CONCAT(x, y)  _CONCAT(x, y)

#define PIPELINE_PREFIX(name) CONCAT(PIPELINE_FUNCTION_PREFIX, name)

#define PIPELINE CONCAT(PIPELINE_TYPE_PREFIX, Pipeline)

#define VERTEX_SHADER CONCAT(PIPELINE_TYPE_PREFIX, VertexShader)
#define GEOM_SHADER   CONCAT(PIPELINE_TYPE_PREFIX, GeometryShader)
#define PIXEL_SHADER  CONCAT(PIPELINE_TYPE_PREFIX, PixelShader)

#define VERTEX PIPELINE_VERTEX_TYPE
#define VS_OUT PIPELINE_VS_OUT_TYPE
#define GS_OUT PIPELINE_GS_OUT_TYPE
#define MESH   PIPELINE_MESH_TYPE

typedef void (*VERTEX_SHADER)(const VERTEX* in, VS_OUT* out);
typedef void (*GEOM_SHADER)(const VS_OUT* in0,
                            const VS_OUT* in1,
                            const VS_OUT* in2,
                            GS_OUT* out0,
                            GS_OUT* out1,
                            GS_OUT* out2,
                            size_t triangle_index);
typedef Color (*PIXEL_SHADER)(const GS_OUT* in);

typedef struct
{
  Graphics* graphics;
  VERTEX_SHADER vertex_shader;
  GEOM_SHADER geom_shader;
  PIXEL_SHADER pixel_shader;
} PIPELINE;

void PIPELINE_PREFIX(pipeline_init)(PIPELINE* pipeline,
                                    Graphics* graphics,
                                    VERTEX_SHADER vertex_shader,
                                    GEOM_SHADER geom_shader,
                                    PIXEL_SHADER pixel_shader);
void PIPELINE_PREFIX(pipeline_draw)(PIPELINE* pipeline, const MESH* mesh);

#else

static void pipeline_transform_to_screen(PIPELINE* pipeline, GS_OUT* v);

static void pipeline_draw_triangle(PIPELINE* pipeline, const GS_OUT* v0, const GS_OUT* v1, const GS_OUT* v2);
static void pipeline_draw_triangle_flat_bottom(PIPELINE* pipeline,
                                               const GS_OUT* v0,
                                               const GS_OUT* v1,
                                               const GS_OUT* v2);
static void pipeline_draw_triangle_flat_top(PIPELINE* pipeline, const GS_OUT* v0, const GS_OUT* v1, const GS_OUT* v2);
static void pipeline_draw_triangle_flat(PIPELINE* pipeline,
                                        const GS_OUT* left_start,
                                        const GS_OUT* right_start,
                                        const GS_OUT* left_inc,
                                        const GS_OUT* right_inc,
                                        float height);

static void swap(const GS_OUT** v, const GS_OUT** w);

void PIPELINE_PREFIX(pipeline_init)(PIPELINE* pipeline,
                                    Graphics* graphics,
                                    VERTEX_SHADER vertex_shader,
                                    GEOM_SHADER geom_shader,
                                    PIXEL_SHADER pixel_shader)
{
  pipeline->graphics = graphics;
  pipeline->vertex_shader = vertex_shader;
  pipeline->geom_shader = geom_shader;
  pipeline->pixel_shader = pixel_shader;
}

void PIPELINE_PREFIX(pipeline_draw)(PIPELINE* pipeline, const MESH* mesh)
{
  DynList trans_verts;
  dyn_list_init(&trans_verts, sizeof(VS_OUT));

  for (size_t i = 0; i < mesh->vertices.size; i++) {
    const VERTEX* v = dyn_list_at(&mesh->vertices, i);
    VS_OUT* vs = dyn_list_add_slot(&trans_verts);
    pipeline->vertex_shader(v, vs);
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
    pipeline->geom_shader(vs0, vs1, vs2, &gs0, &gs1, &gs2, i);

    pipeline_transform_to_screen(pipeline, &gs0);
    pipeline_transform_to_screen(pipeline, &gs1);
    pipeline_transform_to_screen(pipeline, &gs2);

    pipeline_draw_triangle(pipeline, &gs0, &gs1, &gs2);
  }

  dyn_list_destroy(&trans_verts);
}

static void pipeline_transform_to_screen(PIPELINE* pipeline, GS_OUT* v)
{
  const size_t screen_width = pipeline->graphics->screen_width;
  const size_t screen_height = pipeline->graphics->screen_height;
  v->pos.x = (screen_width / 2.0f) * (v->pos.x + 1.0f);
  v->pos.y = (screen_height / 2.0f) * (-v->pos.y + 1.0f);
}

static void pipeline_draw_triangle(PIPELINE* pipeline, const GS_OUT* v0, const GS_OUT* v1, const GS_OUT* v2)
{
  if (v0->pos.y > v1->pos.y) swap(&v0, &v1);
  if (v0->pos.y > v2->pos.y) swap(&v0, &v2);
  if (v1->pos.y > v2->pos.y) swap(&v1, &v2);

  GS_OUT q;
  GS_OUT_INTERPOLATE(&q, v0, v2, (v1->pos.y - v0->pos.y) / (v2->pos.y - v0->pos.y));

  if (q.pos.x > v1->pos.x) {
    pipeline_draw_triangle_flat_bottom(pipeline, v0, v1, &q);
    pipeline_draw_triangle_flat_top(pipeline, v1, &q, v2);
  } else {
    pipeline_draw_triangle_flat_bottom(pipeline, v0, &q, v1);
    pipeline_draw_triangle_flat_top(pipeline, &q, v1, v2);
  }
}

// `v1` and `v2` form the flat bottom of the triangle.
static void pipeline_draw_triangle_flat_bottom(PIPELINE* pipeline, const GS_OUT* v0, const GS_OUT* v1, const GS_OUT* v2)
{
  const float height = v1->pos.y - v0->pos.y;

  // (v1 - v0) / height
  GS_OUT left_inc;
  GS_OUT_SUB(&left_inc, v1, v0);
  GS_OUT_MUL(&left_inc, &left_inc, 1.0f / height);

  // (v2 - v0) / height
  GS_OUT right_inc;
  GS_OUT_SUB(&right_inc, v2, v0);
  GS_OUT_MUL(&right_inc, &right_inc, 1.0f / height);

  pipeline_draw_triangle_flat(pipeline, v0, v0, &left_inc, &right_inc, height);
}

// `v0` and `v1` form the flat top of the triangle.
static void pipeline_draw_triangle_flat_top(PIPELINE* pipeline, const GS_OUT* v0, const GS_OUT* v1, const GS_OUT* v2)
{
  const float height = v2->pos.y - v0->pos.y;

  // (v2 - v0) / height
  GS_OUT left_inc;
  GS_OUT_SUB(&left_inc, v2, v0);
  GS_OUT_MUL(&left_inc, &left_inc, 1.0f / height);

  // (v2 - v1) / height
  GS_OUT right_inc;
  GS_OUT_SUB(&right_inc, v2, v1);
  GS_OUT_MUL(&right_inc, &right_inc, 1.0f / height);

  pipeline_draw_triangle_flat(pipeline, v0, v1, &left_inc, &right_inc, height);
}

static void pipeline_draw_triangle_flat(PIPELINE* pipeline,
                                        const GS_OUT* left_start,
                                        const GS_OUT* right_start,
                                        const GS_OUT* left_inc,
                                        const GS_OUT* right_inc,
                                        float height)
{
  GS_OUT left = *left_start;
  GS_OUT right = *right_start;

  const int y_start = ceil(left.pos.y - 0.5f);
  const int y_end = ceil(left.pos.y + height - 0.5f);

  // Move to starting positions.
  GS_OUT_MUL_ADD(&left, &left, left_inc, y_start + 0.5f - left.pos.y);
  GS_OUT_MUL_ADD(&right, &right, right_inc, y_start + 0.5f - right.pos.y);

  for (int y = y_start; y < y_end; y++) {
    const int x_start = ceil(left.pos.x - 0.5f);
    const int x_end = ceil(right.pos.x - 0.5f);

    const float width = right.pos.x - left.pos.x;

    for (int x = x_start; x < x_end; x++) {
      GS_OUT scan;
      GS_OUT_INTERPOLATE(&scan, &left, &right, (x + 0.5f - left.pos.x) / width);

      const Color color = pipeline->pixel_shader(&scan);
      graphics_set_pixel(pipeline->graphics, x, y, color);
    }

    GS_OUT_ADD(&left, &left, left_inc);
    GS_OUT_ADD(&right, &right, right_inc);
  }
}

static void swap(const GS_OUT** v, const GS_OUT** w)
{
  const GS_OUT* temp = *v;
  *v = *w;
  *w = temp;
}

#endif
