#ifndef TEXTURE_PIPELINE_H_
#define TEXTURE_PIPELINE_H_

#include "effects/texture_effect.h"
#include "meshes/texture_mesh.h"

#define PIPELINE_TYPE_PREFIX            Texture
#define PIPELINE_FUNCTION_PREFIX        texture_
#define PIPELINE_MESH_TYPE              TextureMesh
#define PIPELINE_EFFECT_TYPE            TextureEffect
#define PIPELINE_EFFECT_FUNCTION_PREFIX texture_effect_

#include "pipeline.inc"

#endif
