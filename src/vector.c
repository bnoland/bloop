#include "vector.h"

Vec2 vec2_make(float x, float y)
{
  Vec2 result;
  result.x = x;
  result.y = y;
  return result;
}

Vec2 vec2_add(const Vec2* v, const Vec2* w)
{
  Vec2 result;
  result.x = v->x + w->x;
  result.y = v->y + w->y;
  return result;
}

Vec2 vec2_sub(const Vec2* v, const Vec2* w)
{
  Vec2 result;
  result.x = v->x - w->x;
  result.y = v->y - w->y;
  return result;
}

Vec2 vec2_mul(const Vec2* v, float c)
{
  Vec2 result;
  result.x = v->x * c;
  result.y = v->y * c;
  return result;
}

Vec2 vec2_mul_add(const Vec2* v, const Vec2* w, float c)
{
  Vec2 result;
  result.x = v->x + w->x * c;
  result.y = v->y + w->y * c;
  return result;
}

Vec2 vec2_interpolate(const Vec2* v, const Vec2* w, float alpha)
{
  Vec2 result;
  result.x = (1.0f - alpha) * v->x + alpha * w->x;
  result.y = (1.0f - alpha) * v->y + alpha * w->y;
  return result;
}

float vec2_dot(const Vec2* v, const Vec2* w)
{
  return v->x * w->x + v->y * w->y;
}

Vec3 vec3_make(float x, float y, float z)
{
  Vec3 result;
  result.x = x;
  result.y = y;
  result.z = z;
  return result;
}

Vec3 vec3_add(const Vec3* v, const Vec3* w)
{
  Vec3 result;
  result.x = v->x + w->x;
  result.y = v->y + w->y;
  result.z = v->z + w->z;
  return result;
}

Vec3 vec3_sub(const Vec3* v, const Vec3* w)
{
  Vec3 result;
  result.x = v->x - w->x;
  result.y = v->y - w->y;
  result.z = v->z - w->z;
  return result;
}

Vec3 vec3_mul(const Vec3* v, float c)
{
  Vec3 result;
  result.x = v->x * c;
  result.y = v->y * c;
  result.z = v->z * c;
  return result;
}

Vec3 vec3_mul_add(const Vec3* v, const Vec3* w, float c)
{
  Vec3 result;
  result.x = v->x + c * w->x;
  result.y = v->y + c * w->y;
  result.z = v->z + c * w->z;
  return result;
}

Vec3 vec3_interpolate(const Vec3* v, const Vec3* w, float alpha)
{
  Vec3 result;
  result.x = (1.0f - alpha) * v->x + alpha * w->x;
  result.y = (1.0f - alpha) * v->y + alpha * w->y;
  result.z = (1.0f - alpha) * v->z + alpha * w->z;
  return result;
}

float vec3_dot(const Vec3* v, const Vec3* w)
{
  return v->x * w->x + v->y * w->y + v->z * w->z;
}

Vec4 vec4_make(float x, float y, float z, float w)
{
  Vec4 result;
  result.x = x;
  result.y = y;
  result.z = z;
  result.w = w;
  return result;
}

Vec4 vec4_add(const Vec4* v, const Vec4* w)
{
  Vec4 result;
  result.x = v->x + w->x;
  result.y = v->y + w->y;
  result.z = v->z + w->z;
  result.w = v->w + w->w;
  return result;
}

Vec4 vec4_sub(const Vec4* v, const Vec4* w)
{
  Vec4 result;
  result.x = v->x - w->x;
  result.y = v->y - w->y;
  result.z = v->z - w->z;
  result.w = v->w - w->w;
  return result;
}

Vec4 vec4_mul(const Vec4* v, float c)
{
  Vec4 result;
  result.x = v->x * c;
  result.y = v->y * c;
  result.z = v->z * c;
  result.w = v->w * c;
  return result;
}

Vec4 vec4_mul_add(const Vec4* v, const Vec4* w, float c)
{
  Vec4 result;
  result.x = v->x + c * w->x;
  result.y = v->y + c * w->y;
  result.z = v->z + c * w->z;
  result.w = v->w + c * w->w;
  return result;
}

Vec4 vec4_interpolate(const Vec4* v, const Vec4* w, float alpha)
{
  Vec4 result;
  result.x = (1.0f - alpha) * v->x + alpha * w->x;
  result.y = (1.0f - alpha) * v->y + alpha * w->y;
  result.z = (1.0f - alpha) * v->z + alpha * w->z;
  result.w = (1.0f - alpha) * v->w + alpha * w->w;
  return result;
}
