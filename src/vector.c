#include "vector.h"

void vec2_add(Vec2* dest, const Vec2* v, const Vec2* w)
{
  dest->x = v->x + w->x;
  dest->y = v->y + w->y;
}

void vec2_sub(Vec2* dest, const Vec2* v, const Vec2* w)
{
  dest->x = v->x - w->x;
  dest->y = v->y - w->y;
}

void vec2_mul(Vec2* dest, const Vec2* v, float c)
{
  dest->x = v->x * c;
  dest->y = v->y * c;
}

float vec2_dot(const Vec2* v, const Vec2* w)
{
  return v->x * w->x + v->y * w->y;
}

void vec3_add(Vec3* dest, const Vec3* v, const Vec3* w)
{
  dest->x = v->x + w->x;
  dest->y = v->y + w->y;
  dest->z = v->z + w->z;
}

void vec3_sub(Vec3* dest, const Vec3* v, const Vec3* w)
{
  dest->x = v->x - w->x;
  dest->y = v->y - w->y;
  dest->z = v->z - w->z;
}

void vec3_mul(Vec3* dest, const Vec3* v, float c)
{
  dest->x = v->x * c;
  dest->y = v->y * c;
  dest->z = v->z * c;
}

float vec3_dot(const Vec3* v, const Vec3* w)
{
  return v->x * w->x + v->y * w->y + v->z * w->z;
}

void vec4_add(Vec4* dest, const Vec4* v, const Vec4* w)
{
  dest->x = v->x + w->x;
  dest->y = v->y + w->y;
  dest->z = v->z + w->z;
  dest->w = v->w + w->w;
}

void vec4_sub(Vec4* dest, const Vec4* v, const Vec4* w)
{
  dest->x = v->x - w->x;
  dest->y = v->y - w->y;
  dest->z = v->z - w->z;
  dest->w = v->w - w->w;
}

void vec4_mul(Vec4* dest, const Vec4* v, float c)
{
  dest->x = v->x * c;
  dest->y = v->y * c;
  dest->z = v->z * c;
  dest->w = v->w * c;
}
