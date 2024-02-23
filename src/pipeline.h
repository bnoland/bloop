#ifndef PIPELINE_H_
#define PIPELINE_H_

#include "graphics.h"
#include "vertex.h"
#include "mesh.h"

#include <stddef.h>

typedef void (*VertexShaderFunction)(Vertex* v);
typedef void (*GeometryShaderFunction)(Vertex* v0, Vertex* v1, Vertex* v2, size_t triangle_index);
typedef Color (*PixelShaderFunction)(Vertex* v);

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
void pipeline_draw(Pipeline* pipeline, const Mesh* mesh);

#endif
