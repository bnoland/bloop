#ifndef TEXTURE_MESH_H_
#define TEXTURE_MESH_H_

#include "vector.h"

typedef struct
{
  Vec3 pos;
  Vec2 uv;
} TextureVertex;

#define MESH_TYPE_PREFIX        Texture
#define MESH_FUNCTION_PREFIX    texture_
#define MESH_VERTEX_TYPE        TextureVertex
#define MESH_VERTEX_HAS_UVS     true
#define MESH_VERTEX_HAS_NORMALS false

#include "mesh.inc"

#endif
