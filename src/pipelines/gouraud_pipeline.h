#ifndef GOURAUD_PIPELINE_H_
#define GOURAUD_PIPELINE_H_

#include "effects/gouraud_effect.h"
#include "meshes/gouraud_mesh.h"

#define PIPELINE_TYPE_PREFIX            Gouraud
#define PIPELINE_FUNCTION_PREFIX        gouraud_
#define PIPELINE_MESH_TYPE              GouraudMesh
#define PIPELINE_EFFECT_TYPE            GouraudEffect
#define PIPELINE_EFFECT_FUNCTION_PREFIX gouraud_effect_

#include "pipeline.inc"

#endif
