#ifndef COLOR_MESH_H_
#define COLOR_MESH_H_

#include "effects/color_effect.h"

#define MESH_TYPE_PREFIX        Color
#define MESH_FUNCTION_PREFIX    color_
#define MESH_VERTEX_TYPE        ColorEffectVertex
#define MESH_VERTEX_HAS_UVS     false
#define MESH_VERTEX_HAS_NORMALS false

#include "mesh.inc"

#endif
