#ifndef DEPTH_BUFFER_H_
#define DEPTH_BUFFER_H_

#include <stdbool.h>

typedef struct
{
  int width;
  int height;
  float* values;
} DepthBuffer;

DepthBuffer* depth_buffer_make(int width, int height);
void depth_buffer_destroy(DepthBuffer* buffer);
void depth_buffer_clear(const DepthBuffer* buffer);
float depth_buffer_at(const DepthBuffer* buffer, int x, int y);
bool depth_buffer_test_and_set(const DepthBuffer* buffer, int x, int y, float depth);

#endif
