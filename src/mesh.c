#include "mesh.h"

#include <stdlib.h>

void mesh_initialize(Mesh* mesh)
{
  mesh->vertices = NULL;
  mesh->indices = NULL;
  mesh->num_vertices = 0;
  mesh->num_indices = 0;
}

void mesh_free(Mesh* mesh)
{
  free(mesh->vertices);
  free(mesh->indices);
  mesh->vertices = NULL;
  mesh->indices = NULL;
  mesh->num_vertices = 0;
  mesh->num_indices = 0;
}

void mesh_set_vertices(Mesh* mesh, const Vec3 vertices[], size_t num_vertices)
{
  mesh->vertices = (Vec3*)malloc(num_vertices * sizeof(Vec3));
  mesh->num_vertices = num_vertices;
  for (size_t i = 0; i < num_vertices; i++) {
    mesh->vertices[i] = vertices[i];
  }
}

void mesh_set_indices(Mesh* mesh, const size_t indices[], size_t num_indices)
{
  mesh->indices = (size_t*)malloc(num_indices * sizeof(size_t));
  mesh->num_indices = num_indices;
  for (size_t i = 0; i < num_indices; i++) {
    mesh->indices[i] = indices[i];
  }
}

bool mesh_load_from_file(const char* path)
{
  // XXX: Need an OBJ file loader.
  return false;
}
