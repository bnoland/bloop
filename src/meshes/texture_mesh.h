#ifndef TEXTURE_MESH_H_
#define TEXTURE_MESH_H_

#include "effects/texture_effect.h"

#define MESH_TYPE_PREFIX        Texture
#define MESH_FUNCTION_PREFIX    texture_
#define MESH_VERTEX_TYPE        TextureEffectVertex
#define MESH_VERTEX_HAS_UVS     true
#define MESH_VERTEX_HAS_NORMALS false

#include "mesh.inc"

#endif
