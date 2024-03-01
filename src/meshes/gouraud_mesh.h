#ifndef GOURAUD_MESH_H_
#define GOURAUD_MESH_H_

#include "effects/gouraud_effect.h"

#define MESH_TYPE_PREFIX        Gouraud
#define MESH_FUNCTION_PREFIX    gouraud_
#define MESH_VERTEX_TYPE        GouraudEffectVertex
#define MESH_VERTEX_HAS_UVS     false
#define MESH_VERTEX_HAS_NORMALS true

#include "mesh.inc"

#endif
