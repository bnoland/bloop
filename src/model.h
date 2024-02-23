#ifndef OBJ_H_
#define OBJ_H_

#include "dynlist.h"
#include "vector.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
  size_t pos_index;
  size_t uv_index;
  size_t normal_index;
  bool has_uv;
  bool has_normal;
} FaceElement;

typedef struct
{
  FaceElement elements[3];
} Face;

typedef struct
{
  DynList faces;
  DynList positions;
  DynList uvs;
  DynList normals;
} Model;

bool model_load(Model* model, const char* path, bool initialize);
void model_init(Model* model);
void model_destroy(Model* model);
const Face* model_get_face(const Model* model, size_t index);
const Vec3* model_get_position(const Model* model, size_t index);
const Vec2* model_get_uv(const Model* model, size_t index);
const Vec3* model_get_normal(const Model* model, size_t index);

#endif
