#ifndef PIPELINE_H_
#define PIPELINE_H_

#include "vector.h"
#include "mesh.h"

// XXX: Allow for more general vertices.
typedef void (*VertexShaderFunction)(const Vec3* v);
typedef void (*GeometryShaderFunction)(const Vec3* v0, const Vec3* v1, const Vec3* v2);
typedef void (*PixelShaderFunction)(const Vec3* v);

typedef struct
{
  VertexShaderFunction vertex_shader;
  GeometryShaderFunction geom_shader;
  PixelShaderFunction pixel_shader;
} Pipeline;

void pipeline_init(Pipeline* pipeline,
                   VertexShaderFunction vertex_shader,
                   GeometryShaderFunction geom_shader,
                   PixelShaderFunction pixel_shader);
void pipeline_draw(const Mesh* mesh);

#endif
