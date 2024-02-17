#include "model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void model_init(Model* model)
{
  model->faces = NULL;
  model->vertices = NULL;
  model->uvs = NULL;
  model->normals = NULL;
  model->num_faces = 0;
  model->num_vertices = 0;
  model->num_uvs = 0;
  model->num_normals = 0;
}

bool model_load(Model* model, const char* path)
{
  model_init(model);

  size_t max_faces = 0;
  size_t max_vertices = 0;
  size_t max_uvs = 0;
  size_t max_normals = 0;

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

        if (model->num_vertices >= max_vertices) {
          max_vertices = 2 * (max_vertices + 1);
          model->vertices = (Vec3*)realloc(model->vertices, max_vertices * sizeof(Vec3));
        }

        model->vertices[model->num_vertices++] = vertex;
      } else if (line[1] == 'n') {  // Vertex normal
        Vec3 normal;
        if (sscanf(&line[2], "%f %f %f", &normal.x, &normal.y, &normal.z) != 3) {
          fprintf(stderr, "Failed to read vertex normal: %s, line %zu\n", path, line_number);
          return false;
        }

        if (model->num_normals >= max_normals) {
          max_normals = 2 * (max_normals + 1);
          model->normals = (Vec3*)realloc(model->normals, max_normals * sizeof(Vec3));
        }

        model->normals[model->num_normals++] = normal;
      } else if (line[1] == 't') {  // Texture coordinate
        Vec2 uv;
        if (sscanf(&line[2], "%f %f", &uv.x, &uv.y) != 2) {
          fprintf(stderr, "Failed to read texture coordinate: %s, line %zu\n", path, line_number);
          return false;
        }

        if (model->num_uvs >= max_uvs) {
          max_uvs = 2 * (max_uvs + 1);
          model->uvs = (Vec2*)realloc(model->uvs, max_uvs * sizeof(Vec2));
        }

        model->uvs[model->num_uvs++] = uv;
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

      if (model->num_faces >= max_faces) {
        max_faces = 2 * (max_faces + 1);
        model->faces = (Face*)realloc(model->faces, max_faces * sizeof(Face));
      }

      model->faces[model->num_faces++] = face;
    }
  }

  fclose(input);
  return true;
}

void model_free(Model* model)
{}
