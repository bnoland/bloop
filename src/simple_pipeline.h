#ifndef SIMPLE_PIPELINE_H_
#define SIMPLE_PIPELINE_H_

#include "vertex.h"
#include "simple_mesh.h"

#define PIPELINE_TYPE_PREFIX     Simple
#define PIPELINE_FUNCTION_PREFIX simple_

#define PIPELINE_VERTEX_TYPE Vertex
#define PIPELINE_VS_OUT_TYPE Vertex
#define PIPELINE_GS_OUT_TYPE Vertex
#define PIPELINE_MESH_TYPE   SimpleMesh

#define GS_OUT_ADD         vertex_add
#define GS_OUT_SUB         vertex_sub
#define GS_OUT_MUL         vertex_mul
#define GS_OUT_MUL_ADD     vertex_mul_add
#define GS_OUT_INTERPOLATE vertex_interpolate

#include "pipeline.inc"

#endif
