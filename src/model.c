#include "model.h"

#include "vector.h"
#include "utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void model_init(Model* model)
{
  dyn_list_init(&model->faces, sizeof(Face));
  dyn_list_init(&model->positions, sizeof(Vec3));
  dyn_list_init(&model->uvs, sizeof(Vec2));
  dyn_list_init(&model->normals, sizeof(Vec3));
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

    // Position
    if (strncmp(line, "v ", 2) == 0) {
      Vec3 pos;
      if (sscanf(&line[2], "%f %f %f", &pos.x, &pos.y, &pos.z) != 3) {
        fprintf(stderr, "Failed to read position: %s, line %zu\n", path, line_number);
        return false;
      }
      dyn_list_add(&model->positions, &pos);
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

    // Normal
    if (strncmp(line, "vn ", 3) == 0) {
      Vec3 normal;
      if (sscanf(&line[2], "%f %f %f", &normal.x, &normal.y, &normal.z) != 3) {
        fprintf(stderr, "Failed to read normal: %s, line %zu\n", path, line_number);
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

        // Read position index
        const char* pos_index = strsep(&ptr, "/");
        if (pos_index == NULL) {
          fprintf(stderr, "Failed to read face position index: %s, line %zu\n", path, line_number);
          return false;
        }
        sscanf(pos_index, "%zu", &face.elements[i].pos_index);
        face.elements[i].pos_index--;  // Make zero-based

        // Read (optional) texture coordinate index
        const char* uv_index = strsep(&ptr, "/");
        if (uv_index == NULL || uv_index[0] == '\0') {
          face.elements[i].has_uv = false;
          face.elements[i].uv_index = 0;
        } else {
          face.elements[i].has_uv = true;
          sscanf(uv_index, "%zu", &face.elements[i].uv_index);
          face.elements[i].uv_index--;  // Make zero-based
        }

        // Read (optional) normal index
        const char* normal_index = strsep(&ptr, "/");
        if (normal_index == NULL || normal_index[0] == '\0') {
          face.elements[i].has_normal = false;
          face.elements[i].normal_index = 0;
        } else {
          face.elements[i].has_normal = true;
          sscanf(normal_index, "%zu", &face.elements[i].normal_index);
          face.elements[i].normal_index--;  // Make zero-based
        }
      }

      dyn_list_add(&model->faces, &face);
      continue;
    }
  }

  fclose(input);
  return true;
}

void model_destroy(Model* model)
{
  dyn_list_destroy(&model->faces);
  dyn_list_destroy(&model->positions);
  dyn_list_destroy(&model->uvs);
  dyn_list_destroy(&model->normals);
}

const Face* model_get_face(const Model* model, size_t index)
{
  assert(index < model->faces.size);
  return (Face*)dyn_list_at(&model->faces, index);
}

const Vec3* model_get_position(const Model* model, size_t index)
{
  assert(index < model->positions.size);
  return (Vec3*)dyn_list_at(&model->positions, index);
}

const Vec2* model_get_uv(const Model* model, size_t index)
{
  assert(index < model->uvs.size);
  return (Vec2*)dyn_list_at(&model->uvs, index);
}

const Vec3* model_get_normal(const Model* model, size_t index)
{
  assert(index < model->normals.size);
  return (Vec3*)dyn_list_at(&model->normals, index);
}
