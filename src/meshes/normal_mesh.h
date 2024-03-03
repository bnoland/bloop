#ifndef NORMAL_MESH_H_
#define NORMAL_MESH_H_

#include "vector.h"

typedef struct
{
  Vec3 pos;
  Vec3 normal;
} NormalVertex;

#define MESH_TYPE_PREFIX        Normal
#define MESH_FUNCTION_PREFIX    normal_
#define MESH_VERTEX_TYPE        NormalVertex
#define MESH_VERTEX_HAS_UVS     false
#define MESH_VERTEX_HAS_NORMALS true

#include "mesh.inc"

#endif
