#include "mesh.h"

#include "vector.h"
#include "model.h"

void mesh_initialize(Mesh* mesh)
{
  dyn_list_initialize(&mesh->vertices, sizeof(Vertex));
  dyn_list_initialize(&mesh->positions, sizeof(Vec3));
  dyn_list_initialize(&mesh->uvs, sizeof(Vec2));
  dyn_list_initialize(&mesh->normals, sizeof(Vec3));
}

void mesh_free(Mesh* mesh)
{
  dyn_list_free(&mesh->vertices);
  dyn_list_free(&mesh->positions);
  dyn_list_free(&mesh->uvs);
  dyn_list_free(&mesh->normals);
}

bool mesh_load_from_file(Mesh* mesh, const char* path, bool initialize, bool load_uvs, bool load_normals)
{
  if (initialize) mesh_initialize(mesh);

  Model model;
  model_load(&model, path, true);

  // XXX: Should maybe abstract the APIs a little...

  const size_t num_faces = model.faces.size;
  for (size_t i = 0; i < num_faces; i++) {
    const Face* face = (Face*)dyn_list_at(&model.faces, i);
    for (size_t j = 0; j < 3; j++) {
      Vertex vertex;
      // Model indices are 1-based, hence the -1.
      vertex.pos_index = face->elements[j].vertex_index - 1;
      if (load_uvs) {
        vertex.uv_index = face->elements[j].uv_index - 1;
      }
      if (load_normals) {
        vertex.normal_index = face->elements[j].normal_index - 1;
      }
      dyn_list_add(&mesh->vertices, &vertex);
    }
  }

  const size_t num_vertices = model.vertices.size;
  for (size_t i = 0; i < num_vertices; i++) {
    const Vec3* position = (Vec3*)dyn_list_at(&model.vertices, i);
    dyn_list_add(&mesh->positions, position);
  }

  if (load_uvs) {
    const size_t num_uvs = model.uvs.size;
    for (size_t i = 0; i < num_uvs; i++) {
      const Vec2* uv = (Vec2*)dyn_list_at(&model.uvs, i);
      dyn_list_add(&mesh->uvs, uv);
    }
  }

  if (load_normals) {
    const size_t num_normals = model.normals.size;
    for (size_t i = 0; i < num_normals; i++) {
      const Vec3* normal = (Vec3*)dyn_list_at(&model.normals, i);
      dyn_list_add(&mesh->normals, normal);
    }
  }

  model_free(&model);
  return false;
}
