#ifndef MATRIX_H_
#define MATRIX_H_

#include "vector.h"

typedef struct
{
  float elements[3][3];
} Mat3;

void mat3_zero(Mat3* dest);
void mat3_identity(Mat3* dest);
void mat3_add(Mat3* dest, const Mat3* a, const Mat3* b);
void mat3_sub(Mat3* dest, const Mat3* a, const Mat3* b);
void mat3_mul(Mat3* dest, const Mat3* a, const Mat3* b);
void mat3_scalar_mul(Mat3* dest, const Mat3* a, float c);
void mat3_vec_mul(Vec3* dest, const Mat3* a, const Vec3* v);

typedef struct
{
  float elements[4][4];
} Mat4;

void mat4_zero(Mat4* dest);
void mat4_identity(Mat4* dest);
void mat4_projection(Mat4* dest, float fov, float aspect_ratio, float near, float far);
void mat4_rotation_x(Mat4* dest, float angle);
void mat4_rotation_y(Mat4* dest, float angle);
void mat4_rotation_z(Mat4* dest, float angle);
void mat4_translation(Mat4* dest, float x, float y, float z);
void mat4_add(Mat4* dest, const Mat4* a, const Mat4* b);
void mat4_sub(Mat4* dest, const Mat4* a, const Mat4* b);
void mat4_mul(Mat4* dest, const Mat4* a, const Mat4* b);
void mat4_scalar_mul(Mat4* dest, const Mat4* a, float c);
void mat4_vec_mul(Vec4* dest, const Mat4* a, const Vec4* v);

#endif
