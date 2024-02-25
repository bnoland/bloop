#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct
{
  union
  {
    struct
    {
      float x;
      float y;
    };
    float elements[2];
  };
} Vec2;

Vec2 vec2_make(float x, float y);
Vec2 vec2_add(const Vec2* v, const Vec2* w);
Vec2 vec2_sub(const Vec2* v, const Vec2* w);
Vec2 vec2_mul(const Vec2* v, float c);
Vec2 vec2_mul_add(const Vec2* v, const Vec2* w, float c);
Vec2 vec2_interpolate(const Vec2* v, const Vec2* w, float alpha);
float vec2_dot(const Vec2* v, const Vec2* w);

typedef struct
{
  union
  {
    struct
    {
      float x;
      float y;
      float z;
    };
    float elements[3];
  };
} Vec3;

Vec3 vec3_make(float x, float y, float z);
Vec3 vec3_add(const Vec3* v, const Vec3* w);
Vec3 vec3_sub(const Vec3* v, const Vec3* w);
Vec3 vec3_mul(const Vec3* v, float c);
Vec3 vec3_mul_add(const Vec3* v, const Vec3* w, float c);
Vec3 vec3_interpolate(const Vec3* v, const Vec3* w, float alpha);
float vec3_dot(const Vec3* v, const Vec3* w);

typedef struct
{
  union
  {
    struct
    {
      float x;
      float y;
      float z;
      float w;
    };
    float elements[4];
  };
} Vec4;

Vec4 vec4_make(float x, float y, float z, float w);
Vec4 vec4_add(const Vec4* v, const Vec4* w);
Vec4 vec4_sub(const Vec4* v, const Vec4* w);
Vec4 vec4_mul(const Vec4* v, float c);
Vec4 vec4_mul_add(const Vec4* v, const Vec4* w, float c);
Vec4 vec4_interpolate(const Vec4* v, const Vec4* w, float alpha);

#endif
