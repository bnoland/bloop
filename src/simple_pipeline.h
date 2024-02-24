#ifndef SIMPLE_PIPELINE_H_
#define SIMPLE_PIPELINE_H_

#include "vertex.h"
#include "simple_mesh.h"

#define PIPELINE_TYPE_PREFIX     Simple
#define PIPELINE_FUNCTION_PREFIX simple_

#define PIPELINE_VERTEX_TYPE Vertex
#define PIPELINE_VS_OUT_TYPE VSOut
#define PIPELINE_GS_OUT_TYPE GSOut
#define PIPELINE_MESH_TYPE   SimpleMesh

#define GS_OUT_ADD         gsout_add
#define GS_OUT_SUB         gsout_sub
#define GS_OUT_MUL         gsout_mul
#define GS_OUT_MUL_ADD     gsout_mul_add
#define GS_OUT_INTERPOLATE gsout_interpolate

#include "pipeline.inc"

#endif
