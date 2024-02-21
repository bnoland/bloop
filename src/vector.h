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

void vec2_add(Vec2* dest, const Vec2* v, const Vec2* w);
void vec2_sub(Vec2* dest, const Vec2* v, const Vec2* w);
void vec2_mul(Vec2* dest, const Vec2* v, float c);
void vec2_mul_add(Vec2* dest, const Vec2* v, const Vec2* w, float c);
void vec2_interpolate(Vec2* dest, const Vec2* v, const Vec2* w, float alpha);
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

void vec3_add(Vec3* dest, const Vec3* v, const Vec3* w);
void vec3_sub(Vec3* dest, const Vec3* v, const Vec3* w);
void vec3_mul(Vec3* dest, const Vec3* v, float c);
void vec3_mul_add(Vec3* dest, const Vec3* v, const Vec3* w, float c);
void vec3_interpolate(Vec3* dest, const Vec3* v, const Vec3* w, float alpha);
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

void vec4_add(Vec4* dest, const Vec4* v, const Vec4* w);
void vec4_sub(Vec4* dest, const Vec4* v, const Vec4* w);
void vec4_mul(Vec4* dest, const Vec4* v, float c);

#endif
