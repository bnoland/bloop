#ifndef OBJ_H_
#define OBJ_H_

#include "vector.h"

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
  Face* faces;
  Vec3* vertices;
  Vec2* uvs;
  Vec3* normals;
  size_t num_faces;
  size_t num_vertices;
  size_t num_uvs;
  size_t num_normals;
} Model;

bool model_load(Model* model, const char* path);
void model_init(Model* model);
void model_free(Model* model);

#endif
