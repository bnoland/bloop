#ifndef MESH_H_
#define MESH_H_

#include "dynlist.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
  size_t pos_index;
  size_t uv_index;
  size_t normal_index;
} Vertex;

typedef struct
{
  DynList vertices;
  DynList positions;
  DynList uvs;
  DynList normals;
} Mesh;

void mesh_init(Mesh* mesh);
void mesh_destroy(Mesh* mesh);
bool mesh_load_from_file(Mesh* mesh, const char* path, bool initialize, bool load_uvs, bool load_normals);

#endif
