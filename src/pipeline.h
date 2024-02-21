#ifndef PIPELINE_H_
#define PIPELINE_H_

#include "dynlist.h"
#include "graphics.h"

#include <stddef.h>

// XXX: Vertex interpolation routines.
typedef struct
{
  Vec3 pos;
  Vec2 uv;
  Vec3 normal;
} Vertex;

typedef void (*VertexShaderFunction)(Vertex* v);
typedef void (*GeometryShaderFunction)(Vertex* v0, Vertex* v1, Vertex* v2, size_t triangle_index);
typedef void (*PixelShaderFunction)(Vertex* v);

typedef struct
{
  Graphics* graphics;
  VertexShaderFunction vertex_shader;
  GeometryShaderFunction geom_shader;
  PixelShaderFunction pixel_shader;
} Pipeline;

void pipeline_init(Pipeline* pipeline,
                   Graphics* graphics,
                   VertexShaderFunction vertex_shader,
                   GeometryShaderFunction geom_shader,
                   PixelShaderFunction pixel_shader);
void pipeline_draw(Pipeline* pipeline, const DynList* vertices, const DynList* indices);

#endif
