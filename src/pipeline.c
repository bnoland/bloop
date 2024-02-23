#include "pipeline.h"

#include "dynlist.h"

#include <tgmath.h>

static void default_vertex_shader(Vertex* v)
{
  // XXX: So the mesh fits on the screen. Remove later.
  v->pos.x /= 2.0f;
  v->pos.y /= 2.0f;
}

static void default_geom_shader(Vertex* v0, Vertex* v1, Vertex* v2, size_t triangle_index)
{
  (void)v0;
  (void)v1;
  (void)v2;
  (void)triangle_index;
}

static Color default_pixel_shader(Vertex* v)
{
  (void)v;
  return 0xffffffff;
}

static void pipeline_draw_triangle_flat(Pipeline* pipeline,
                                        const Vertex* left_start,
                                        const Vertex* right_start,
                                        const Vertex* left_inc,
                                        const Vertex* right_inc,
                                        float height)
{
  Vertex left = *left_start;
  Vertex right = *right_start;

  const int y_start = ceil(left.pos.y - 0.5f);
  const int y_end = ceil(left.pos.y + height - 0.5f);

  // Move to starting positions.
  vertex_mul_add(&left, &left, left_inc, y_start + 0.5f - left.pos.y);
  vertex_mul_add(&right, &right, right_inc, y_start + 0.5f - right.pos.y);

  for (int y = y_start; y < y_end; y++) {
    const int x_start = ceil(left.pos.x - 0.5f);
    const int x_end = ceil(right.pos.x - 0.5f);

    const float width = right.pos.x - left.pos.x;

    for (int x = x_start; x < x_end; x++) {
      Vertex scan;
      vertex_interpolate(&scan, &left, &right, (x + 0.5f - left.pos.x) / width);

      const Color color = pipeline->pixel_shader(&scan);
      graphics_set_pixel(pipeline->graphics, x, y, color);
    }

    vertex_add(&left, &left, left_inc);
    vertex_add(&right, &right, right_inc);
  }
}

// `v1` and `v2` form the flat bottom of the triangle.
static void pipeline_draw_triangle_flat_bottom(Pipeline* pipeline, const Vertex* v0, const Vertex* v1, const Vertex* v2)
{
  const float height = v1->pos.y - v0->pos.y;

  // (v1 - v0) / height
  Vertex left_inc;
  vertex_sub(&left_inc, v1, v0);
  vertex_mul(&left_inc, &left_inc, 1.0f / height);

  // (v2 - v0) / height
  Vertex right_inc;
  vertex_sub(&right_inc, v2, v0);
  vertex_mul(&right_inc, &right_inc, 1.0f / height);

  pipeline_draw_triangle_flat(pipeline, v0, v0, &left_inc, &right_inc, height);
}

// `v0` and `v1` form the flat top of the triangle.
static void pipeline_draw_triangle_flat_top(Pipeline* pipeline, const Vertex* v0, const Vertex* v1, const Vertex* v2)
{
  const float height = v2->pos.y - v0->pos.y;

  // (v2 - v0) / height
  Vertex left_inc;
  vertex_sub(&left_inc, v2, v0);
  vertex_mul(&left_inc, &left_inc, 1.0f / height);

  // (v2 - v1) / height
  Vertex right_inc;
  vertex_sub(&right_inc, v2, v1);
  vertex_mul(&right_inc, &right_inc, 1.0f / height);

  pipeline_draw_triangle_flat(pipeline, v0, v1, &left_inc, &right_inc, height);
}

static void swap(const Vertex** v, const Vertex** w)
{
  const Vertex* temp = *v;
  *v = *w;
  *w = temp;
}

static void pipeline_draw_triangle(Pipeline* pipeline, const Vertex* v0, const Vertex* v1, const Vertex* v2)
{
  if (v0->pos.y > v1->pos.y) swap(&v0, &v1);
  if (v0->pos.y > v2->pos.y) swap(&v0, &v2);
  if (v1->pos.y > v2->pos.y) swap(&v1, &v2);

  Vertex q;
  vertex_interpolate(&q, v0, v2, (v1->pos.y - v0->pos.y) / (v2->pos.y - v0->pos.y));

  if (q.pos.x > v1->pos.x) {
    pipeline_draw_triangle_flat_bottom(pipeline, v0, v1, &q);
    pipeline_draw_triangle_flat_top(pipeline, v1, &q, v2);
  } else {
    pipeline_draw_triangle_flat_bottom(pipeline, v0, &q, v1);
    pipeline_draw_triangle_flat_top(pipeline, &q, v1, v2);
  }
}

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

static void pipeline_transform_to_screen(Pipeline* pipeline, Vertex* v)
{
  const size_t screen_width = pipeline->graphics->screen_width;
  const size_t screen_height = pipeline->graphics->screen_height;
  v->pos.x = (screen_width / 2.0f) * (v->pos.x + 1.0f);
  v->pos.y = (screen_height / 2.0f) * (-v->pos.y + 1.0f);
}

void pipeline_draw(Pipeline* pipeline, const Mesh* mesh)
{
  DynList vertices;
  dyn_list_init(&vertices, sizeof(Vertex));

  for (size_t i = 0; i < mesh->vertices.size; i++) {
    const MeshVertex* mesh_vertex = dyn_list_at(&mesh->vertices, i);
    Vertex vertex = {
      .pos = mesh_vertex->pos,
      .uv = mesh_vertex->uv,
      .normal = mesh_vertex->normal,
    };
    pipeline->vertex_shader(&vertex);
    dyn_list_add(&vertices, &vertex);
  }

  for (size_t i = 0; i < mesh->indices.size / 3; i++) {
    const size_t* i0 = dyn_list_at(&mesh->indices, 3 * i);
    const size_t* i1 = dyn_list_at(&mesh->indices, 3 * i + 1);
    const size_t* i2 = dyn_list_at(&mesh->indices, 3 * i + 2);

    const Vertex* v0 = dyn_list_at(&vertices, *i0);
    const Vertex* v1 = dyn_list_at(&vertices, *i1);
    const Vertex* v2 = dyn_list_at(&vertices, *i2);

    // The geometry shader may modify the vertices, so we pass it copies.
    Vertex v0_copy = *v0;
    Vertex v1_copy = *v1;
    Vertex v2_copy = *v2;
    pipeline->geom_shader(&v0_copy, &v1_copy, &v2_copy, i);

    pipeline_transform_to_screen(pipeline, &v0_copy);
    pipeline_transform_to_screen(pipeline, &v1_copy);
    pipeline_transform_to_screen(pipeline, &v2_copy);

    pipeline_draw_triangle(pipeline, &v0_copy, &v1_copy, &v2_copy);
  }

  dyn_list_destroy(&vertices);
}
