#ifndef MATRIX_H_
#define MATRIX_H_

#include "vector.h"

typedef struct
{
  float elements[3][3];
} Mat3;

Mat3 mat3_zero(void);
Mat3 mat3_identity(void);
Mat3 mat3_add(const Mat3* a, const Mat3* b);
Mat3 mat3_sub(const Mat3* a, const Mat3* b);
Mat3 mat3_mul(const Mat3* a, const Mat3* b);
Mat3 mat3_scalar_mul(const Mat3* a, float c);
Vec3 mat3_vec_mul(const Mat3* a, const Vec3* v);

typedef struct
{
  float elements[4][4];
} Mat4;

Mat4 mat4_zero(void);
Mat4 mat4_identity(void);
Mat4 mat4_projection(float fov, float aspect_ratio, float near, float far);
Mat4 mat4_rotation_x(float angle);
Mat4 mat4_rotation_y(float angle);
Mat4 mat4_rotation_z(float angle);
Mat4 mat4_translation(float x, float y, float z);
Mat4 mat4_add(const Mat4* a, const Mat4* b);
Mat4 mat4_sub(const Mat4* a, const Mat4* b);
Mat4 mat4_mul(const Mat4* a, const Mat4* b);
Mat4 mat4_scalar_mul(const Mat4* a, float c);
Vec4 mat4_vec_mul(const Mat4* a, const Vec4* v);

#endif
