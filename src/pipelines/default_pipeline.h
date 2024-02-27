#ifndef DEFAULT_PIPELINE_H_
#define DEFAULT_PIPELINE_H_

#include "effects/default_effect.h"
#include "meshes/default_mesh.h"

#define PIPELINE_TYPE_PREFIX            Default
#define PIPELINE_FUNCTION_PREFIX        default_
#define PIPELINE_MESH_TYPE              DefaultMesh
#define PIPELINE_EFFECT_TYPE            DefaultEffect
#define PIPELINE_EFFECT_FUNCTION_PREFIX default_effect_

#include "pipeline.inc"

#endif
