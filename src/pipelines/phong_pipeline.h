#ifndef PHONG_PIPELINE_H_
#define PHONG_PIPELINE_H_

#include "effects/phong_effect.h"
#include "meshes/normal_mesh.h"

#define PIPELINE_TYPE_PREFIX            Phong
#define PIPELINE_FUNCTION_PREFIX        phong_
#define PIPELINE_MESH_TYPE              NormalMesh
#define PIPELINE_EFFECT_TYPE            PhongEffect
#define PIPELINE_EFFECT_FUNCTION_PREFIX phong_effect_

#include "pipeline.inc"

#endif
