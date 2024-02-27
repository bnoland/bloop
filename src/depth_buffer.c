#include "depth_buffer.h"

#include <stdlib.h>
#include <assert.h>
#include <math.h>

DepthBuffer* depth_buffer_make(int width, int height)
{
  DepthBuffer* buffer = malloc(sizeof(DepthBuffer));
  buffer->width = width;
  buffer->height = height;
  buffer->values = malloc(width * height * sizeof(float));
  return buffer;
}

void depth_buffer_destroy(DepthBuffer* buffer)
{
  free(buffer->values);
  free(buffer);
}

void depth_buffer_clear(const DepthBuffer* buffer)
{
  for (int y = 0; y < buffer->height; y++) {
    for (int x = 0; x < buffer->width; x++) {
      buffer->values[x + y * buffer->width] = -INFINITY;
    }
  }
}

float depth_buffer_at(const DepthBuffer* buffer, int x, int y)
{
  assert(x >= 0 && x < buffer->width);
  assert(y >= 0 && y < buffer->height);
  return buffer->values[x + y * buffer->width];
}

bool depth_buffer_test_and_set(const DepthBuffer* buffer, int x, int y, float depth)
{
  assert(x >= 0 && x < buffer->width);
  assert(y >= 0 && y < buffer->height);

  if (buffer->values[x + y * buffer->width] < depth) {
    buffer->values[x + y * buffer->width] = depth;
    return true;
  }

  return false;
}
