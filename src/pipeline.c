#include "pipeline.h"

static void pipeline_process_vertices(Pipeline* pipeline, const DynList* vertices, const DynList* indices);
static void pipeline_assemble_triangles(Pipeline* pipeline, const DynList* vertices, const DynList* indices);
static void pipeline_process_triangle(Pipeline* pipeline,
                                      const Vertex* v0,
                                      const Vertex* v1,
                                      const Vertex* v2,
                                      size_t triangle_index);
static void pipeline_post_process_triangle_vertices(Pipeline* pipeline, Vertex* v0, Vertex* v1, Vertex* v2);
static void pipeline_draw_triangle(Pipeline* pipeline, const Vertex* v0, const Vertex* v1, const Vertex* v2);
static void pipeline_transform_to_screen(Pipeline* pipeline, Vertex* v);

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

  pipeline_draw_triangle(pipeline, v0, v1, v2);
}

static void pipeline_draw_triangle(Pipeline* pipeline, const Vertex* v0, const Vertex* v1, const Vertex* v2)
{
  // XXX: Need to call pixel shader while drawing triangle.
  graphics_draw_triangle(pipeline->graphics, &v0->pos, &v1->pos, &v2->pos, 0xffffffff);
}

static void pipeline_transform_to_screen(Pipeline* pipeline, Vertex* v)
{
  const size_t screen_width = pipeline->graphics->screen_width;
  const size_t screen_height = pipeline->graphics->screen_height;
  v->pos.x = (screen_width / 2.0f) * (v->pos.x + 1.0f);
  v->pos.y = (screen_height / 2.0f) * (-v->pos.y + 1.0f);
}

static void default_vertex_shader(Vertex* v)
{}

static void default_geom_shader(Vertex* v0, Vertex* v1, Vertex* v2, size_t triangle_index)
{}

static void default_pixel_shader(Vertex* v)
{}
