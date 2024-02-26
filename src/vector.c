#include "vector.h"

Vec2 vec2_make(float x, float y)
{
  return (Vec2){ .x = x, .y = y };
}

Vec2 vec2_add(const Vec2* v, const Vec2* w)
{
  return (Vec2){
    .x = v->x + w->x,
    .y = v->y + w->y,
  };
}

Vec2 vec2_sub(const Vec2* v, const Vec2* w)
{
  return (Vec2){
    .x = v->x - w->x,
    .y = v->y - w->y,
  };
}

Vec2 vec2_mul(const Vec2* v, float c)
{
  return (Vec2){
    .x = v->x * c,
    .y = v->y * c,
  };
}

Vec2 vec2_mul_add(const Vec2* v, const Vec2* w, float c)
{
  return (Vec2){
    .x = v->x + w->x * c,
    .y = v->y + w->y * c,
  };
}

Vec2 vec2_interpolate(const Vec2* v, const Vec2* w, float alpha)
{
  return (Vec2){
    .x = (1.0f - alpha) * v->x + alpha * w->x,
    .y = (1.0f - alpha) * v->y + alpha * w->y,
  };
}

float vec2_dot(const Vec2* v, const Vec2* w)
{
  return v->x * w->x + v->y * w->y;
}

Vec3 vec3_make(float x, float y, float z)
{
  return (Vec3){ .x = x, .y = y, .z = z };
}

Vec3 vec3_add(const Vec3* v, const Vec3* w)
{
  return (Vec3){
    .x = v->x + w->x,
    .y = v->y + w->y,
    .z = v->z + w->z,
  };
}

Vec3 vec3_sub(const Vec3* v, const Vec3* w)
{
  return (Vec3){
    .x = v->x - w->x,
    .y = v->y - w->y,
    .z = v->z - w->z,
  };
}

Vec3 vec3_mul(const Vec3* v, float c)
{
  return (Vec3){
    .x = v->x * c,
    .y = v->y * c,
    .z = v->z * c,
  };
}

Vec3 vec3_mul_add(const Vec3* v, const Vec3* w, float c)
{
  return (Vec3){
    .x = v->x + c * w->x,
    .y = v->y + c * w->y,
    .z = v->z + c * w->z,
  };
}

Vec3 vec3_interpolate(const Vec3* v, const Vec3* w, float alpha)
{
  return (Vec3){
    .x = (1.0f - alpha) * v->x + alpha * w->x,
    .y = (1.0f - alpha) * v->y + alpha * w->y,
    .z = (1.0f - alpha) * v->z + alpha * w->z,
  };
}

float vec3_dot(const Vec3* v, const Vec3* w)
{
  return v->x * w->x + v->y * w->y + v->z * w->z;
}

Vec4 vec4_make(float x, float y, float z, float w)
{
  return (Vec4){ .x = x, .y = y, .z = z, .w = w };
}

Vec4 vec4_add(const Vec4* v, const Vec4* w)
{
  return (Vec4){
    .x = v->x + w->x,
    .y = v->y + w->y,
    .z = v->z + w->z,
    .w = v->w + w->w,
  };
}

Vec4 vec4_sub(const Vec4* v, const Vec4* w)
{
  return (Vec4){
    .x = v->x - w->x,
    .y = v->y - w->y,
    .z = v->z - w->z,
    .w = v->w - w->w,
  };
}

Vec4 vec4_mul(const Vec4* v, float c)
{
  return (Vec4){
    .x = v->x * c,
    .y = v->y * c,
    .z = v->z * c,
    .w = v->w * c,
  };
}

Vec4 vec4_mul_add(const Vec4* v, const Vec4* w, float c)
{
  return (Vec4){
    .x = v->x + c * w->x,
    .y = v->y + c * w->y,
    .z = v->z + c * w->z,
    .w = v->w + c * w->w,
  };
}

Vec4 vec4_interpolate(const Vec4* v, const Vec4* w, float alpha)
{
  return (Vec4){
    .x = (1.0f - alpha) * v->x + alpha * w->x,
    .y = (1.0f - alpha) * v->y + alpha * w->y,
    .z = (1.0f - alpha) * v->z + alpha * w->z,
    .w = (1.0f - alpha) * v->w + alpha * w->w,
  };
}
