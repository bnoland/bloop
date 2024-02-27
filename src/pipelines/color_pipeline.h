#ifndef COLOR_PIPELINE_H_
#define COLOR_PIPELINE_H_

#include "effects/color_effect.h"
#include "meshes/color_mesh.h"

#define PIPELINE_TYPE_PREFIX            Color
#define PIPELINE_FUNCTION_PREFIX        color_
#define PIPELINE_MESH_TYPE              ColorMesh
#define PIPELINE_EFFECT_TYPE            ColorEffect
#define PIPELINE_EFFECT_FUNCTION_PREFIX color_effect_

#include "pipeline.inc"

#endif
