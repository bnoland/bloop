#include "vertex.h"

void vertex_init(Vertex* v, const MeshVertex* mv)
{
  vec4_from_vec3(&v->pos, &mv->pos, 1.0f);
  v->uv = mv->uv;
  v->normal = mv->normal;
}

void vertex_add(Vertex* dest, const Vertex* v, const Vertex* w)
{
  vec4_add(&dest->pos, &v->pos, &w->pos);
  vec2_add(&dest->uv, &v->uv, &w->uv);
  vec3_add(&dest->normal, &v->normal, &w->normal);
}

void vertex_sub(Vertex* dest, const Vertex* v, const Vertex* w)
{
  vec4_sub(&dest->pos, &v->pos, &w->pos);
  vec2_sub(&dest->uv, &v->uv, &w->uv);
  vec3_sub(&dest->normal, &v->normal, &w->normal);
}

void vertex_mul(Vertex* dest, const Vertex* v, float c)
{
  vec4_mul(&dest->pos, &v->pos, c);
  vec2_mul(&dest->uv, &v->uv, c);
  vec3_mul(&dest->normal, &v->normal, c);
}

void vertex_mul_add(Vertex* dest, const Vertex* v, const Vertex* w, float c)
{
  vec4_mul_add(&dest->pos, &v->pos, &w->pos, c);
  vec2_mul_add(&dest->uv, &v->uv, &w->uv, c);
  vec3_mul_add(&dest->normal, &v->normal, &w->normal, c);
}

void vertex_interpolate(Vertex* dest, const Vertex* v, const Vertex* w, float alpha)
{
  vec4_interpolate(&dest->pos, &v->pos, &w->pos, alpha);
  vec2_interpolate(&dest->uv, &v->uv, &w->uv, alpha);
  vec3_interpolate(&dest->normal, &v->normal, &w->normal, alpha);
}
