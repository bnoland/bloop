#ifndef VERTEX_H_
#define VERTEX_H_

#include "vector.h"

typedef struct
{
  Vec3 pos;
} Vertex;

typedef struct
{
  Vec4 pos;
} VSOut;

typedef struct
{
  Vec4 pos;
} GSOut;

GSOut gsout_add(const GSOut* v, const GSOut* w);
GSOut gsout_sub(const GSOut* v, const GSOut* w);
GSOut gsout_mul(const GSOut* v, float c);
GSOut gsout_mul_add(const GSOut* v, const GSOut* w, float c);
GSOut gsout_interpolate(const GSOut* v, const GSOut* w, float alpha);

#endif
