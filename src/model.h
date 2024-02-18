#ifndef OBJ_H_
#define OBJ_H_

#include "dynlist.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
  size_t vertex_index;
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
  DynList vertices;
  DynList uvs;
  DynList normals;
} Model;

bool model_load(Model* model, const char* path);
void model_init(Model* model);
void model_free(Model* model);

#endif
