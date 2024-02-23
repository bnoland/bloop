#ifndef VERTEX_H_
#define VERTEX_H_

#include "vector.h"
#include "mesh.h"

typedef struct
{
  Vec3 pos;
  Vec2 uv;
  Vec3 normal;
} Vertex;

void vertex_init(Vertex* v, const MeshVertex* mv);
void vertex_add(Vertex* dest, const Vertex* v, const Vertex* w);
void vertex_sub(Vertex* dest, const Vertex* v, const Vertex* w);
void vertex_mul(Vertex* dest, const Vertex* v, float c);
void vertex_mul_add(Vertex* dest, const Vertex* v, const Vertex* w, float c);
void vertex_interpolate(Vertex* dest, const Vertex* v, const Vertex* w, float alpha);

#endif
