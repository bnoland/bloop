#include "mesh.h"

#include "model.h"

#include <stdio.h>
#include <stdbool.h>

void mesh_init(Mesh* mesh)
{
  dyn_list_init(&mesh->vertices, sizeof(MeshVertex));
  dyn_list_init(&mesh->indices, sizeof(size_t));
}

void mesh_destroy(Mesh* mesh)
{
  dyn_list_destroy(&mesh->vertices);
  dyn_list_destroy(&mesh->indices);
}

static bool face_elements_equal(const FaceElement* a, const FaceElement* b)
{
  if (a->pos_index != b->pos_index) return false;
  if (a->has_uv != b->has_uv) return false;
  if (a->has_uv && a->uv_index != b->uv_index) return false;
  if (a->has_normal != b->has_normal) return false;
  if (a->has_normal && a->normal_index != b->normal_index) return false;
  return true;
}

bool mesh_load_from_file(Mesh* mesh, const char* path, bool initialize, bool load_uvs, bool load_normals)
{
  if (initialize) mesh_init(mesh);

  Model model;
  if (!model_load(&model, path, true)) {
    fprintf(stderr, "Failed to load model: %s\n", path);
    return false;
  }

  DynList positions;
  dyn_list_init(&positions, sizeof(Vec3));
  for (size_t i = 0; i < model.positions.size; i++) {
    dyn_list_add(&positions, model_get_position(&model, i));
  }

  DynList uvs;
  dyn_list_init(&uvs, sizeof(Vec2));
  if (load_uvs) {
    for (size_t i = 0; i < model.uvs.size; i++) {
      dyn_list_add(&uvs, model_get_uv(&model, i));
    }
  }

  DynList normals;
  dyn_list_init(&normals, sizeof(Vec3));
  if (load_normals) {
    for (size_t i = 0; i < model.normals.size; i++) {
      dyn_list_add(&normals, model_get_normal(&model, i));
    }
  }

  // XXX: Replace with a better data structure.
  DynList face_elements_seen;
  dyn_list_init(&face_elements_seen, sizeof(FaceElement));

  size_t current_index = 0;

  for (size_t i = 0; i < model.faces.size; i++) {
    const Face* face = model_get_face(&model, i);

    for (size_t j = 0; j < 3; j++) {

      // XXX: Make a `dyn_list_search()` function that returns an index.
      bool seen_face_element = false;
      for (size_t index = 0; index < face_elements_seen.size; index++) {
        const FaceElement* element = (const FaceElement*)dyn_list_at(&face_elements_seen, index);
        if (face_elements_equal(element, &face->elements[j])) {
          dyn_list_add(&mesh->indices, &index);
          seen_face_element = true;
          break;
        }
      }

      if (seen_face_element) continue;

      dyn_list_add(&face_elements_seen, &face->elements[j]);

      MeshVertex vertex;

      vertex.pos = *model_get_position(&model, face->elements[j].pos_index);

      if (load_uvs) {
        if (!face->elements[j].has_uv) {
          fprintf(stderr, "Face %zu, element %zu is missing texture coordinate\n", i, j);
          return false;
        }
        vertex.uv = *model_get_uv(&model, face->elements[j].uv_index);
      }

      if (load_normals) {
        if (!face->elements[j].has_normal) {
          fprintf(stderr, "Face %zu, element %zu is missing normal\n", i, j);
          return false;
        }
        vertex.normal = *model_get_normal(&model, face->elements[j].normal_index);
      }

      dyn_list_add(&mesh->vertices, &vertex);
      dyn_list_add(&mesh->indices, &current_index);
      current_index++;
    }
  }

  dyn_list_destroy(&positions);
  dyn_list_destroy(&uvs);
  dyn_list_destroy(&normals);
  dyn_list_destroy(&face_elements_seen);

  model_destroy(&model);
  return true;
}
