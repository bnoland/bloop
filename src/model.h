#ifndef OBJ_H_
#define OBJ_H_

#include "dynlist.h"
#include "vector.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
  // All indices are 1-based. A value of 0 indicates no corresponding index.
  size_t vertex_index;
  size_t uv_index;  // optional
  size_t normal_index;  // optional
} FaceElement;

typedef struct
{
  FaceElement elements[3];
} Face;

typedef struct
{
  DynList faces;
  DynList vertices;
  DynList uvs;
  DynList normals;
} Model;

bool model_load(Model* model, const char* path, bool initialize);
void model_inititalize(Model* model);
void model_free(Model* model);
const Face* model_get_face(const Model* model, size_t index);
const Vec3* model_get_vertex(const Model* model, size_t index);
const Vec2* model_get_uv(const Model* model, size_t index);
const Vec3* model_get_normal(const Model* model, size_t index);

#endif
