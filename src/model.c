#include "model.h"

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

bool model_load(Model* model, const char* path)
{
  model_init(model);

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

    if (line[0] == 'v') {
      if (line[1] == ' ') {  // Vertex position
        Vec3 vertex;
        if (sscanf(&line[2], "%f %f %f", &vertex.x, &vertex.y, &vertex.z) != 3) {
          fprintf(stderr, "Failed to read vertex position: %s, line %zu\n", path, line_number);
          return false;
        }
        dyn_list_add(&model->vertices, &vertex);
      } else if (line[1] == 'n') {  // Vertex normal
        Vec3 normal;
        if (sscanf(&line[2], "%f %f %f", &normal.x, &normal.y, &normal.z) != 3) {
          fprintf(stderr, "Failed to read vertex normal: %s, line %zu\n", path, line_number);
          return false;
        }
        dyn_list_add(&model->normals, &normal);
      } else if (line[1] == 't') {  // Texture coordinate
        Vec2 uv;
        if (sscanf(&line[2], "%f %f", &uv.x, &uv.y) != 2) {
          fprintf(stderr, "Failed to read texture coordinate: %s, line %zu\n", path, line_number);
          return false;
        }
        dyn_list_add(&model->uvs, &uv);
      }
    } else if (line[0] == 'f') {  // Face
      char elem_strs[3][sizeof(line)];
      if (sscanf(&line[2], "%s %s %s", elem_strs[0], elem_strs[1], elem_strs[2]) != 3) {
        fprintf(stderr, "Failed to read face elements: %s, line %zu\n", path, line_number);
        return false;
      }

      Face face;

      for (size_t i = 0; i < 3; i++) {
        char* src = elem_strs[i];

        for (size_t j = 0; j < 3; j++) {
          char idx_str[sizeof(elem_strs[i])];
          char* dest = idx_str;
          if (*src == '/') src++;
          while (*src != '\0' && *src != '/') *dest++ = *src++;
          *dest = '\0';

          if (j == 0) {
            sscanf(idx_str, "%zu", &face.elements[i].vertex_index);
          } else if (j == 1) {
            if (dest == idx_str) {
              face.elements[i].has_uv = false;
            } else {
              face.elements[i].has_uv = true;
              sscanf(idx_str, "%zu", &face.elements[i].uv_index);
            }
          } else {
            if (dest == idx_str) {
              face.elements[i].has_normal = false;
            } else {
              face.elements[i].has_normal = true;
              sscanf(idx_str, "%zu", &face.elements[i].normal_index);
            }
          }
        }
      }

      dyn_list_add(&model->faces, &face);
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
