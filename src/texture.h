#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "graphics.h"

#include <stdbool.h>

typedef struct
{
  int width;
  int height;
  Color* data;
} Texture;

Texture* texture_make(void);
bool texture_load_from_file(Texture* texture, const char* path);
void texture_destroy(Texture* texture);
Color texture_at(const Texture* texture, int x, int y);
Color texture_uv_at(const Texture* texture, float u, float v);

#endif
