
// Bail if we're missing any of the required parameters.
#ifndef MESH_TYPE_PREFIX
#error "MESH_TYPE_PREFIX is undefined"
#endif
#ifndef MESH_FUNCTION_PREFIX
#error "MESH_FUNCTION_PREFIX is undefined"
#endif
#ifndef MESH_VERTEX_TYPE
#error "MESH_VERTEX_TYPE is undefined"
#endif
#ifndef MESH_VERTEX_HAS_UVS
#error "MESH_VERTEX_HAS_UVS is undefined"
#endif
#ifndef MESH_VERTEX_HAS_NORMALS
#error "MESH_VERTEX_HAS_NORMALS is undefined"
#endif

#ifndef MESH_IMPLEMENTATION

#include "dynlist.h"

#include <stdbool.h>

#define _CONCAT(x, y) x##y
#define CONCAT(x, y)  _CONCAT(x, y)

#define MESH_PREFIX(name) CONCAT(MESH_FUNCTION_PREFIX, name)

#define MESH   CONCAT(MESH_TYPE_PREFIX, Mesh)
#define VERTEX MESH_VERTEX_TYPE

typedef struct
{
  DynList vertices;
  DynList indices;
} MESH;

void MESH_PREFIX(mesh_init)(MESH* mesh);
void MESH_PREFIX(mesh_destroy)(MESH* mesh);
void MESH_PREFIX(mesh_load_from_arrays)(MESH* mesh,
                                        bool initialize,
                                        const VERTEX vertices[],
                                        size_t num_vertices,
                                        const size_t indices[],
                                        size_t num_indices);
bool MESH_PREFIX(mesh_load_from_file)(MESH* mesh, const char* path, bool initialize, bool load_uvs, bool load_normals);

#else

#include "model.h"

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

static bool face_elements_equal(const void* a, const void* b);

void MESH_PREFIX(mesh_init)(MESH* mesh)
{
  dyn_list_init(&mesh->vertices, sizeof(VERTEX));
  dyn_list_init(&mesh->indices, sizeof(size_t));
}

void MESH_PREFIX(mesh_destroy)(MESH* mesh)
{
  dyn_list_destroy(&mesh->vertices);
  dyn_list_destroy(&mesh->indices);
}

void MESH_PREFIX(mesh_load_from_arrays)(MESH* mesh,
                                        bool initialize,
                                        const VERTEX vertices[],
                                        size_t num_vertices,
                                        const size_t indices[],
                                        size_t num_indices)
{
  assert(num_indices % 3 == 0);

  if (initialize) MESH_PREFIX(mesh_init)(mesh);

  for (size_t i = 0; i < num_vertices; i++) {
    dyn_list_add(&mesh->vertices, &vertices[i]);
  }

  for (size_t i = 0; i < num_indices; i++) {
    dyn_list_add(&mesh->indices, &indices[i]);
  }
}

bool MESH_PREFIX(mesh_load_from_file)(MESH* mesh, const char* path, bool initialize, bool load_uvs, bool load_normals)
{
  if (initialize) MESH_PREFIX(mesh_init)(mesh);

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

#if MESH_VERTEX_HAS_UVS
  DynList uvs;
  dyn_list_init(&uvs, sizeof(Vec2));
  if (load_uvs) {
    for (size_t i = 0; i < model.uvs.size; i++) {
      dyn_list_add(&uvs, model_get_uv(&model, i));
    }
  }
#endif

#if MESH_VERTEX_HAS_NORMALS
  DynList normals;
  dyn_list_init(&normals, sizeof(Vec3));
  if (load_normals) {
    for (size_t i = 0; i < model.normals.size; i++) {
      dyn_list_add(&normals, model_get_normal(&model, i));
    }
  }
#endif

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

      VERTEX vertex = {
        .pos = *model_get_position(&model, face->elements[j].pos_index),
      };

#if MESH_VERTEX_HAS_UVS
      if (load_uvs) {
        if (!face->elements[j].has_uv) {
          fprintf(stderr, "Face %zu, element %zu is missing texture coordinate\n", i, j);
          return false;
        }
        vertex.uv = *model_get_uv(&model, face->elements[j].uv_index);
      }
#endif

#if MESH_VERTEX_HAS_NORMALS
      if (load_normals) {
        if (!face->elements[j].has_normal) {
          fprintf(stderr, "Face %zu, element %zu is missing normal\n", i, j);
          return false;
        }
        vertex.normal = *model_get_normal(&model, face->elements[j].normal_index);
      }
#endif

      dyn_list_add(&mesh->vertices, &vertex);
      dyn_list_add(&mesh->indices, &current_index);
      current_index++;
    }
  }

  dyn_list_destroy(&positions);
#if MESH_VERTEX_HAS_UVS
  dyn_list_destroy(&uvs);
#endif
#if MESH_VERTEX_HAS_NORMALS
  dyn_list_destroy(&normals);
#endif
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

#endif