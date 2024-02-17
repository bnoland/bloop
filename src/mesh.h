#ifndef MESH_H_
#define MESH_H_

#include "vector.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
  Vec3* vertices;
  size_t* indices;
  size_t num_vertices;
  size_t num_indices;
} Mesh;

void mesh_initialize(Mesh* mesh);
void mesh_free(Mesh* mesh);
void mesh_set_vertices(Mesh* mesh, const Vec3 vertices[], size_t num_vertices);
void mesh_set_indices(Mesh* mesh, const size_t indices[], size_t num_indices);
bool mesh_load_from_file(const char* path);

#endif
