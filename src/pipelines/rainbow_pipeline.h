#ifndef RAINBOW_PIPELINE_H_
#define RAINBOW_PIPELINE_H_

#include "effects/rainbow_effect.h"
#include "meshes/rainbow_mesh.h"

#define PIPELINE_TYPE_PREFIX            Rainbow
#define PIPELINE_FUNCTION_PREFIX        rainbow_
#define PIPELINE_MESH_TYPE              RainbowMesh
#define PIPELINE_EFFECT_TYPE            RainbowEffect
#define PIPELINE_EFFECT_FUNCTION_PREFIX rainbow_effect_

#include "pipeline.inc"

#endif
