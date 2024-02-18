#include "model.h"

#include "vector.h"
#include "utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void model_init(Model* model)
{
  dyn_list_initialize(&model->faces, sizeof(Face));
  dyn_list_initialize(&model->vertices, sizeof(Vec3));
  dyn_list_initialize(&model->uvs, sizeof(Vec2));
  dyn_list_initialize(&model->normals, sizeof(Vec3));
}

bool model_load(Model* model, const char* path, bool initialize)
{
  if (initialize) model_init(model);

  FILE* input = fopen(path, "r");
  if (input == NULL) {
    fprintf(stderr, "Failed to open model file: %s\n", path);
    return false;
  }

  char line[1024];
  size_t line_number = 0;

  while (fgets(line, sizeof(line), input) != NULL) {
    line_number++;

    if (line[0] == '#') continue;  // Comment

    // Vertex position
    if (strncmp(line, "v ", 2) == 0) {
      Vec3 vertex;
      if (sscanf(&line[2], "%f %f %f", &vertex.x, &vertex.y, &vertex.z) != 3) {
        fprintf(stderr, "Failed to read vertex position: %s, line %zu\n", path, line_number);
        return false;
      }
      dyn_list_add(&model->vertices, &vertex);
      continue;
    }

    // Texture coordinate
    if (strncmp(line, "vt ", 3) == 0) {
      Vec2 uv;
      if (sscanf(&line[2], "%f %f", &uv.x, &uv.y) != 2) {
        fprintf(stderr, "Failed to read texture coordinate: %s, line %zu\n", path, line_number);
        return false;
      }
      dyn_list_add(&model->uvs, &uv);
      continue;
    }

    // Vertex normal
    if (strncmp(line, "vn ", 3) == 0) {
      Vec3 normal;
      if (sscanf(&line[2], "%f %f %f", &normal.x, &normal.y, &normal.z) != 3) {
        fprintf(stderr, "Failed to read vertex normal: %s, line %zu\n", path, line_number);
        return false;
      }
      dyn_list_add(&model->normals, &normal);
      continue;
    }

    // Face
    if (strncmp(line, "f ", 2) == 0) {
      char elem_strs[3][sizeof(line)];
      if (sscanf(&line[2], "%s %s %s", elem_strs[0], elem_strs[1], elem_strs[2]) != 3) {
        fprintf(stderr, "Failed to read face elements: %s, line %zu\n", path, line_number);
        return false;
      }

      Face face;

      for (size_t i = 0; i < 3; i++) {
        char* ptr = elem_strs[i];

        // Read vertex index
        const char* vertex_idx = strsep(&ptr, "/");
        if (vertex_idx == NULL) {
          fprintf(stderr, "Failed to read face vertex index: %s, line %zu\n", path, line_number);
          return false;
        }
        sscanf(vertex_idx, "%zu", &face.elements[i].vertex_index);

        // Read (optional) texture coordinate index
        const char* uv_idx = strsep(&ptr, "/");
        if (uv_idx == NULL || uv_idx[0] == '\0') {
          face.elements[i].has_uv = false;
          face.elements[i].uv_index = 0;
        } else {
          face.elements[i].has_uv = true;
          sscanf(uv_idx, "%zu", &face.elements[i].uv_index);
        }

        // Read (optional) vertex normal index
        const char* normal_idx = strsep(&ptr, "/");
        if (normal_idx == NULL || normal_idx[0] == '\0') {
          face.elements[i].has_normal = false;
          face.elements[i].normal_index = 0;
        } else {
          face.elements[i].has_normal = true;
          sscanf(normal_idx, "%zu", &face.elements[i].normal_index);
        }
      }

      dyn_list_add(&model->faces, &face);
      continue;
    }
  }

  fclose(input);
  return true;
}

void model_free(Model* model)
{
  dyn_list_free(&model->faces);
  dyn_list_free(&model->vertices);
  dyn_list_free(&model->uvs);
  dyn_list_free(&model->normals);
}
