#include "pipeline.h"

#include <tgmath.h>

static void pipeline_process_vertices(Pipeline* pipeline, const DynList* vertices, const DynList* indices);
static void pipeline_assemble_triangles(Pipeline* pipeline, const DynList* vertices, const DynList* indices);
static void pipeline_process_triangle(Pipeline* pipeline,
                                      const Vertex* v0,
                                      const Vertex* v1,
                                      const Vertex* v2,
                                      size_t triangle_index);
static void pipeline_post_process_triangle_vertices(Pipeline* pipeline, Vertex* v0, Vertex* v1, Vertex* v2);
static void pipeline_transform_to_screen(Pipeline* pipeline, Vertex* v);

// XXX: Triangle drawing routines should operate on vertices directly for interpolation.
static void pipeline_draw_triangle(Pipeline* pipeline, const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color);
static void
pipeline_draw_triangle_flat_bottom(Pipeline* pipeline, const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color);
static void
pipeline_draw_triangle_flat_top(Pipeline* pipeline, const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color);
static void pipeline_draw_triangle_flat(Pipeline* pipeline,
                                        const Vec3* left_start,
                                        const Vec3* right_start,
                                        const Vec3* left_inc,
                                        const Vec3* right_inc,
                                        float height,
                                        Color color);

static void swap(const Vec3** v, const Vec3** w);

static void default_vertex_shader(Vertex* v);
static void default_geom_shader(Vertex* v0, Vertex* v1, Vertex* v2, size_t triangle_index);
static void default_pixel_shader(Vertex* v);

void pipeline_init(Pipeline* pipeline,
                   Graphics* graphics,
                   VertexShaderFunction vertex_shader,
                   GeometryShaderFunction geom_shader,
                   PixelShaderFunction pixel_shader)
{
  pipeline->graphics = graphics;
  pipeline->vertex_shader = (vertex_shader == NULL) ? default_vertex_shader : vertex_shader;
  pipeline->geom_shader = (geom_shader == NULL) ? default_geom_shader : geom_shader;
  pipeline->pixel_shader = (pixel_shader == NULL) ? default_pixel_shader : pixel_shader;
}

void pipeline_draw(Pipeline* pipeline, const DynList* vertices, const DynList* indices)
{
  pipeline_process_vertices(pipeline, vertices, indices);
}

static void pipeline_process_vertices(Pipeline* pipeline, const DynList* vertices, const DynList* indices)
{
  DynList trans_verts;
  dyn_list_init(&trans_verts, sizeof(Vertex));

  for (size_t i = 0; i < vertices->size; i++) {
    Vertex* v = (Vertex*)dyn_list_at(vertices, i);
    Vertex* tv = (Vertex*)dyn_list_add(&trans_verts, v);
    default_vertex_shader(tv);
  }

  pipeline_assemble_triangles(pipeline, &trans_verts, indices);

  dyn_list_destroy(&trans_verts);
}

static void pipeline_assemble_triangles(Pipeline* pipeline, const DynList* vertices, const DynList* indices)
{
  for (size_t i = 0; i < indices->size / 3; i++) {
    const size_t* index0 = (size_t*)dyn_list_at(indices, 3 * i);
    const size_t* index1 = (size_t*)dyn_list_at(indices, 3 * i + 1);
    const size_t* index2 = (size_t*)dyn_list_at(indices, 3 * i + 2);

    const Vertex* v0 = (Vertex*)dyn_list_at(vertices, *index0);
    const Vertex* v1 = (Vertex*)dyn_list_at(vertices, *index1);
    const Vertex* v2 = (Vertex*)dyn_list_at(vertices, *index2);

    pipeline_process_triangle(pipeline, v0, v1, v2, i);
  }
}

static void pipeline_process_triangle(Pipeline* pipeline,
                                      const Vertex* v0,
                                      const Vertex* v1,
                                      const Vertex* v2,
                                      size_t triangle_index)
{
  // Need copies since geometry shader and post-process step of pipeline modify input vertices.
  Vertex v0_copy = *v0;
  Vertex v1_copy = *v1;
  Vertex v2_copy = *v2;
  default_geom_shader(&v0_copy, &v1_copy, &v2_copy, triangle_index);
  pipeline_post_process_triangle_vertices(pipeline, &v0_copy, &v1_copy, &v2_copy);
}

static void pipeline_post_process_triangle_vertices(Pipeline* pipeline, Vertex* v0, Vertex* v1, Vertex* v2)
{
  pipeline_transform_to_screen(pipeline, v0);
  pipeline_transform_to_screen(pipeline, v1);
  pipeline_transform_to_screen(pipeline, v2);

  pipeline_draw_triangle(pipeline, &v0->pos, &v1->pos, &v2->pos, 0xffffffff);
}

static void pipeline_transform_to_screen(Pipeline* pipeline, Vertex* v)
{
  const size_t screen_width = pipeline->graphics->screen_width;
  const size_t screen_height = pipeline->graphics->screen_height;
  v->pos.x = (screen_width / 2.0f) * (v->pos.x + 1.0f);
  v->pos.y = (screen_height / 2.0f) * (-v->pos.y + 1.0f);
}

static void pipeline_draw_triangle(Pipeline* pipeline, const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color)
{
  if (p0->y > p1->y) swap(&p0, &p1);
  if (p0->y > p2->y) swap(&p0, &p2);
  if (p1->y > p2->y) swap(&p1, &p2);

  Vec3 q;
  vec3_interpolate(&q, p0, p2, (p1->y - p0->y) / (p2->y - p0->y));

  if (q.x > p1->x) {
    pipeline_draw_triangle_flat_bottom(pipeline, p0, p1, &q, color);
    pipeline_draw_triangle_flat_top(pipeline, p1, &q, p2, color);
  } else {
    pipeline_draw_triangle_flat_bottom(pipeline, p0, &q, p1, color);
    pipeline_draw_triangle_flat_top(pipeline, &q, p1, p2, color);
  }
}

// `p1` and `p2` form the flat bottom of the triangle.
static void
pipeline_draw_triangle_flat_bottom(Pipeline* pipeline, const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color)
{
  const float height = p1->y - p0->y;

  // (p1 - p0) / height
  Vec3 left_inc;
  vec3_sub(&left_inc, p1, p0);
  vec3_mul(&left_inc, &left_inc, 1.0f / height);

  // (p2 - p0) / height
  Vec3 right_inc;
  vec3_sub(&right_inc, p2, p0);
  vec3_mul(&right_inc, &right_inc, 1.0f / height);

  pipeline_draw_triangle_flat(pipeline, p0, p0, &left_inc, &right_inc, height, color);
}

// `p0` and `p1` form the flat top of the triangle.
static void
pipeline_draw_triangle_flat_top(Pipeline* pipeline, const Vec3* p0, const Vec3* p1, const Vec3* p2, Color color)
{
  const float height = p2->y - p0->y;

  // (p2 - p0) / height
  Vec3 left_inc;
  vec3_sub(&left_inc, p2, p0);
  vec3_mul(&left_inc, &left_inc, 1.0f / height);

  // (p2 - p1) / height
  Vec3 right_inc;
  vec3_sub(&right_inc, p2, p1);
  vec3_mul(&right_inc, &right_inc, 1.0f / height);

  pipeline_draw_triangle_flat(pipeline, p0, p1, &left_inc, &right_inc, height, color);
}

static void pipeline_draw_triangle_flat(Pipeline* pipeline,
                                        const Vec3* left_start,
                                        const Vec3* right_start,
                                        const Vec3* left_inc,
                                        const Vec3* right_inc,
                                        float height,
                                        Color color)
{
  Vec3 left = *left_start;
  Vec3 right = *right_start;

  const int y_start = ceil(left.y - 0.5f);
  const int y_end = ceil(left.y + height - 0.5f);

  // Move to starting positions.
  vec3_mul_add(&left, &left, left_inc, y_start + 0.5f - left.y);
  vec3_mul_add(&right, &right, right_inc, y_start + 0.5f - right.y);

  for (int y = y_start; y < y_end; y++) {
    const int x_start = ceil(left.x - 0.5f);
    const int x_end = ceil(right.x - 0.5f);

    for (int x = x_start; x < x_end; x++) {
      // XXX: Call pixel shader.
      graphics_set_pixel(pipeline->graphics, x, y, color);
    }

    vec3_add(&left, &left, left_inc);
    vec3_add(&right, &right, right_inc);
  }
}

static void swap(const Vec3** v, const Vec3** w)
{
  const Vec3* temp = *v;
  *v = *w;
  *w = temp;
}

static void default_vertex_shader(Vertex* v)
{
  // XXX: So the mesh fits on the screen. Remove later.
  v->pos.x /= 2.0f;
  v->pos.y /= 2.0f;
}

static void default_geom_shader(Vertex* v0, Vertex* v1, Vertex* v2, size_t triangle_index)
{}

static void default_pixel_shader(Vertex* v)
{}
