#ifndef MESH_IMPLEMENTATION

#include "dynlist.h"
#include "vector.h"

#include <stdbool.h>

#ifndef MESH_TYPE_PREFIX
typedef struct
{
  Vec3 pos;
} PositionVertex;
#define MESH_TYPE_PREFIX Position
#endif
#ifndef MESH_FUNCTION_PREFIX
#define MESH_FUNCTION_PREFIX position_
#endif
#ifndef MESH_VERTEX_TYPE
#define MESH_VERTEX_TYPE PositionVertex
#endif
#ifndef MESH_VERTEX_HAS_UVS
#define MESH_VERTEX_HAS_UVS false
#endif
#ifndef MESH_VERTEX_HAS_NORMALS
#define MESH_VERTEX_HAS_NORMALS false
#endif

#undef _CONCAT
#undef CONCAT
#undef MESH_PREFIX
#undef MESH
#undef VERTEX

#define _CONCAT(x, y)     x##y
#define CONCAT(x, y)      _CONCAT(x, y)
#define MESH_PREFIX(name) CONCAT(MESH_FUNCTION_PREFIX, name)
#define MESH              CONCAT(MESH_TYPE_PREFIX, Mesh)
#define VERTEX            MESH_VERTEX_TYPE

typedef struct
{
  DynList vertices;
  DynList indices;
} MESH;

MESH MESH_PREFIX(mesh_make)(void);
void MESH_PREFIX(mesh_destroy)(MESH* mesh);
void MESH_PREFIX(mesh_load_from_arrays)(MESH* mesh,
                                        const VERTEX vertices[],
                                        size_t num_vertices,
                                        const size_t indices[],
                                        size_t num_indices);
bool MESH_PREFIX(mesh_load_from_file)(MESH* mesh, const char* path, bool load_uvs, bool load_normals);
#if MESH_VERTEX_HAS_NORMALS
void MESH_PREFIX(mesh_interpolate_normals)(MESH* mesh);
#endif

#else

#include "model.h"

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

static bool face_elements_equal(const void* a, const void* b);

MESH MESH_PREFIX(mesh_make)(void)
{
  return (MESH){
    .vertices = dyn_list_make(sizeof(VERTEX)),
    .indices = dyn_list_make(sizeof(size_t)),
  };
}

void MESH_PREFIX(mesh_destroy)(MESH* mesh)
{
  dyn_list_destroy(&mesh->vertices);
  dyn_list_destroy(&mesh->indices);
}

void MESH_PREFIX(mesh_load_from_arrays)(MESH* mesh,
                                        const VERTEX vertices[],
                                        size_t num_vertices,
                                        const size_t indices[],
                                        size_t num_indices)
{
  assert(num_indices % 3 == 0);

  for (size_t i = 0; i < num_vertices; i++) {
    dyn_list_add(&mesh->vertices, &vertices[i]);
  }

  for (size_t i = 0; i < num_indices; i++) {
    dyn_list_add(&mesh->indices, &indices[i]);
  }
}

bool MESH_PREFIX(mesh_load_from_file)(MESH* mesh, const char* path, bool load_uvs, bool load_normals)
{
  Model model = model_make();
  if (!model_load_from_file(&model, path)) {
    fprintf(stderr, "Failed to load model: %s\n", path);
    return false;
  }

  DynList positions = dyn_list_make(sizeof(Vec3));
  for (size_t i = 0; i < model.positions.size; i++) {
    dyn_list_add(&positions, model_get_position(&model, i));
  }

#if MESH_VERTEX_HAS_UVS
  DynList uvs = dyn_list_make(sizeof(Vec2));
  if (load_uvs) {
    for (size_t i = 0; i < model.uvs.size; i++) {
      dyn_list_add(&uvs, model_get_uv(&model, i));
    }
  }
#endif

#if MESH_VERTEX_HAS_NORMALS
  DynList normals = dyn_list_make(sizeof(Vec3));
  if (load_normals) {
    for (size_t i = 0; i < model.normals.size; i++) {
      dyn_list_add(&normals, model_get_normal(&model, i));
    }
  }
#endif

  // XXX: Replace with a better data structure. Maybe a hash map from face elements -> indices?
  DynList face_elements_seen = dyn_list_make(sizeof(FaceElement));

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

      VERTEX vertex = { .pos = *model_get_position(&model, face->elements[j].pos_index) };

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

#if MESH_VERTEX_HAS_NORMALS
void MESH_PREFIX(mesh_interpolate_normals)(MESH* mesh)
{
  // XXX: Interpolate normals from face normals.
  // See dis code i wrote:
  // https://github.com/bnoland/blip/blob/main/src/indexed_triangle_list.hpp
}
#endif

static bool face_elements_equal(const void* a, const void* b)
{
  const FaceElement* fa = a;
  const FaceElement* fb = b;

  if (fa->pos_index != fb->pos_index) return false;

  if (fa->has_uv != fb->has_uv) return false;
  if (fa->has_uv && fa->uv_index != fb->uv_index) return false;

  if (fa->has_normal != fb->has_normal) return false;
  if (fa->has_normal && fa->normal_index != fb->normal_index) return false;

  return true;
}

#endif
