#ifndef SIMPLE_PIPELINE_H_
#define SIMPLE_PIPELINE_H_

#include "effects/simple_effect.h"
#include "meshes/simple_mesh.h"

#define PIPELINE_TYPE_PREFIX            Simple
#define PIPELINE_FUNCTION_PREFIX        simple_
#define PIPELINE_MESH_TYPE              SimpleMesh
#define PIPELINE_EFFECT_TYPE            SimpleEffect
#define PIPELINE_EFFECT_FUNCTION_PREFIX simple_effect_

#include "pipeline.inc"

#endif
