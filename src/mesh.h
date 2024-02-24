#ifndef MESH_H_
#define MESH_H_

#include "dynlist.h"
#include "vector.h"

#include <stdbool.h>

typedef struct
{
  Vec3 pos;
  Vec2 uv;
  Vec3 normal;
} MeshVertex;

typedef struct
{
  DynList vertices;
  DynList indices;
} Mesh;

void mesh_init(Mesh* mesh);
void mesh_destroy(Mesh* mesh);
void mesh_load_from_arrays(Mesh* mesh,
                           bool initialize,
                           const MeshVertex vertices[],
                           size_t num_vertices,
                           const size_t indices[],
                           size_t num_indices);
bool mesh_load_from_file(Mesh* mesh, const char* path, bool initialize, bool load_uvs, bool load_normals);

#endif
