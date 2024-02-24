#include "mesh.h"

#include "model.h"

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

static bool face_elements_equal(const void* a, const void* b);

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

void mesh_load_from_arrays(Mesh* mesh,
                           bool initialize,
                           const MeshVertex vertices[],
                           size_t num_vertices,
                           const size_t indices[],
                           size_t num_indices)
{
  assert(num_indices % 3 == 0);

  if (initialize) mesh_init(mesh);

  for (size_t i = 0; i < num_vertices; i++) {
    dyn_list_add(&mesh->vertices, &vertices[i]);
  }

  for (size_t i = 0; i < num_indices; i++) {
    dyn_list_add(&mesh->indices, &indices[i]);
  }
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

  // XXX: Replace with a better data structure. Maybe a hash map from face elements -> indices?
  DynList face_elements_seen;
  dyn_list_init(&face_elements_seen, sizeof(FaceElement));

  size_t current_index = 0;

  for (size_t i = 0; i < model.faces.size; i++) {
    const Face* face = model_get_face(&model, i);

    for (size_t j = 0; j < 3; j++) {
      size_t index;
      if (dyn_list_search(&face_elements_seen, &face->elements[j], &index, face_elements_equal)) {
        dyn_list_add(&mesh->indices, &index);
        continue;
      }

      dyn_list_add(&face_elements_seen, &face->elements[j]);

      MeshVertex vertex = {
        .pos = *model_get_position(&model, face->elements[j].pos_index),
      };

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

static bool face_elements_equal(const void* a, const void* b)
{
  const FaceElement* fa = (const FaceElement*)a;
  const FaceElement* fb = (const FaceElement*)b;

  if (fa->pos_index != fb->pos_index) return false;

  if (fa->has_uv != fb->has_uv) return false;
  if (fa->has_uv && fa->uv_index != fb->uv_index) return false;

  if (fa->has_normal != fb->has_normal) return false;
  if (fa->has_normal && fa->normal_index != fb->normal_index) return false;

  return true;
}
