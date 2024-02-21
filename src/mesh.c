#include "mesh.h"

#include "vector.h"
#include "model.h"

#include <stdio.h>

void mesh_init(Mesh* mesh)
{
  dyn_list_init(&mesh->vertices, sizeof(MeshVertex));
  dyn_list_init(&mesh->positions, sizeof(Vec3));
  dyn_list_init(&mesh->uvs, sizeof(Vec2));
  dyn_list_init(&mesh->normals, sizeof(Vec3));
}

void mesh_destroy(Mesh* mesh)
{
  dyn_list_destroy(&mesh->vertices);
  dyn_list_destroy(&mesh->positions);
  dyn_list_destroy(&mesh->uvs);
  dyn_list_destroy(&mesh->normals);
}

bool mesh_load_from_file(Mesh* mesh, const char* path, bool initialize, bool load_uvs, bool load_normals)
{
  if (initialize) mesh_init(mesh);

  Model model;
  if (!model_load(&model, path, true)) {
    fprintf(stderr, "Failed to load model: %s\n", path);
    return false;
  }

  for (size_t i = 0; i < model.faces.size; i++) {
    const Face* face = model_get_face(&model, i);

    for (size_t j = 0; j < 3; j++) {
      MeshVertex vertex;

      // Model indices are 1-based, hence the -1.
      vertex.pos_index = face->elements[j].vertex_index - 1;

      if (load_uvs) {
        const size_t uv_index = face->elements[j].uv_index;
        if (uv_index == 0) {
          fprintf(stderr, "Encountered vertex with missing texture coordinate.\n");
          return false;
        }
        vertex.uv_index = uv_index - 1;
      }

      if (load_normals) {
        const size_t normal_index = face->elements[j].normal_index;
        if (normal_index == 0) {
          fprintf(stderr, "Encountered vertex with missing normal.\n");
          return false;
        }
        vertex.normal_index = normal_index - 1;
      }

      dyn_list_add(&mesh->vertices, &vertex);
    }
  }

  for (size_t i = 0; i < model.vertices.size; i++) {
    dyn_list_add(&mesh->positions, model_get_vertex(&model, i));
  }

  if (load_uvs) {
    for (size_t i = 0; i < model.uvs.size; i++) {
      dyn_list_add(&mesh->uvs, model_get_uv(&model, i));
    }
  }

  if (load_normals) {
    for (size_t i = 0; i < model.normals.size; i++) {
      dyn_list_add(&mesh->normals, model_get_normal(&model, i));
    }
  }

  model_destroy(&model);
  return true;
}
