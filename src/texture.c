#include "texture.h"

#include "stb_image.h"

#include <assert.h>

Texture* texture_make(void)
{
  Texture* texture = malloc(sizeof(Texture));
  texture->width = texture->height = 0;
  texture->data = NULL;
  return texture;
}

bool texture_load_from_file(Texture* texture, const char* path)
{
  int width, height;
  unsigned char* image_data = stbi_load(path, &width, &height, NULL, 4);
  if (image_data == NULL) {
    return false;
  }

  texture->width = width;
  texture->height = height;
  texture->data = malloc(width * height * sizeof(Color));

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      unsigned char* rgba = &image_data[x * 4 + y * width * 4];
      texture->data[x + y * width] = (rgba[0] << 24) | (rgba[1] << 16) | (rgba[2] << 8) | rgba[3];
    }
  }

  stbi_image_free(image_data);
  return true;
}

void texture_destroy(Texture* texture)
{
  free(texture->data);
  free(texture);
}

Color texture_at(const Texture* texture, int x, int y)
{
  assert(x >= 0 && x < texture->width);
  assert(y >= 0 && y < texture->height);
  return texture->data[x + y * texture->width];
}

Color texture_uv_at(const Texture* texture, float u, float v)
{
  assert(u >= 0.0f && u <= 1.0f);
  assert(v >= 0.0f && v <= 1.0f);
  return texture_at(texture, u * (texture->width - 1), v * (texture->height - 1));
}
