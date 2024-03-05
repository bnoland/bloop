#ifndef POSITION_MESH_H_
#define POSITION_MESH_H_

#include "vector.h"

typedef struct
{
  Vec3 pos;
} PositionVertex;

#define MESH_TYPE_PREFIX        Position
#define MESH_FUNCTION_PREFIX    position_
#define MESH_VERTEX_TYPE        PositionVertex
#define MESH_VERTEX_HAS_UVS     false
#define MESH_VERTEX_HAS_NORMALS false

#include "mesh.inc"

#endif
