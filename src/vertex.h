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

void gsout_add(GSOut* dest, const GSOut* v, const GSOut* w);
void gsout_sub(GSOut* dest, const GSOut* v, const GSOut* w);
void gsout_mul(GSOut* dest, const GSOut* v, float c);
void gsout_mul_add(GSOut* dest, const GSOut* v, const GSOut* w, float c);
void gsout_interpolate(GSOut* dest, const GSOut* v, const GSOut* w, float alpha);

#endif
