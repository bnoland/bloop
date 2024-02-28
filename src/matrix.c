#include "matrix.h"

#include "utility.h"

#include <tgmath.h>

Mat3 mat3_zero(void)
{
  Mat3 result;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      result.elements[i][j] = 0.0f;
    }
  }
  return result;
}

Mat3 mat3_identity(void)
{
  Mat3 result;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      result.elements[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
  return result;
}

Mat3 mat3_add(const Mat3* a, const Mat3* b)
{
  Mat3 result;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      result.elements[i][j] = a->elements[i][j] + b->elements[i][j];
    }
  }
  return result;
}

Mat3 mat3_sub(const Mat3* a, const Mat3* b)
{
  Mat3 result;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      result.elements[i][j] = a->elements[i][j] - b->elements[i][j];
    }
  }
  return result;
}

Mat3 mat3_mul(const Mat3* a, const Mat3* b)
{
  Mat3 result;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      float sum = 0.0f;
      for (int k = 0; k < 3; k++) {
        sum += a->elements[i][k] * b->elements[k][j];
      }
      result.elements[i][j] = sum;
    }
  }
  return result;
}

Mat3 mat3_scalar_mul(const Mat3* a, float c)
{
  Mat3 result;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      result.elements[i][j] = a->elements[i][j] * c;
    }
  }
  return result;
}

Vec3 mat3_vec_mul(const Mat3* a, const Vec3* v)
{
  Vec3 result;
  for (int i = 0; i < 3; i++) {
    float sum = 0.0f;
    for (int j = 0; j < 3; j++) {
      sum += a->elements[i][j] * v->elements[j];
    }
    result.elements[i] = sum;
  }
  return result;
}

Mat4 mat4_zero(void)
{
  Mat4 result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result.elements[i][j] = 0.0f;
    }
  }
  return result;
}

Mat4 mat4_identity(void)
{
  Mat4 result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result.elements[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
  return result;
}

Mat4 mat4_projection(float fov, float aspect_ratio, float near, float far)
{
  const float fov_radians = degrees_to_radians(fov);
  const float width = 2.0f * near * tanf(fov_radians / 2.0f);
  const float height = width * (1.0f / aspect_ratio);

  Mat4 result = mat4_zero();
  result.elements[0][0] = 2.0f * near / width;
  result.elements[1][1] = 2.0f * near / height;
  result.elements[2][2] = -(far + near) / (far - near);
  result.elements[2][3] = -2.0f * near * far / (far - near);
  result.elements[3][2] = -1.0f / near;
  return result;
}

Mat4 mat4_rotation_x(float angle)
{
  const double cos_angle = cos(angle);
  const double sin_angle = sin(angle);

  Mat4 result = mat4_identity();
  result.elements[1][1] = cos_angle;
  result.elements[1][2] = -sin_angle;
  result.elements[2][1] = sin_angle;
  result.elements[2][2] = cos_angle;
  return result;
}

Mat4 mat4_rotation_y(float angle)
{
  const double cos_angle = cos(angle);
  const double sin_angle = sin(angle);

  Mat4 result = mat4_identity();
  result.elements[0][0] = cos_angle;
  result.elements[0][2] = sin_angle;
  result.elements[2][0] = -sin_angle;
  result.elements[2][2] = cos_angle;
  return result;
}

Mat4 mat4_rotation_z(float angle)
{
  const double cos_angle = cos(angle);
  const double sin_angle = sin(angle);

  Mat4 result = mat4_identity();
  result.elements[0][0] = cos_angle;
  result.elements[0][1] = -sin_angle;
  result.elements[1][0] = sin_angle;
  result.elements[1][1] = cos_angle;
  return result;
}

Mat4 mat4_translation(float x, float y, float z)
{
  Mat4 result = mat4_identity();
  result.elements[0][3] = x;
  result.elements[1][3] = y;
  result.elements[2][3] = z;
  return result;
}

Mat4 mat4_add(const Mat4* a, const Mat4* b)
{
  Mat4 result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result.elements[i][j] = a->elements[i][j] + b->elements[i][j];
    }
  }
  return result;
}

Mat4 mat4_sub(const Mat4* a, const Mat4* b)
{
  Mat4 result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result.elements[i][j] = a->elements[i][j] - b->elements[i][j];
    }
  }
  return result;
}

Mat4 mat4_mul(const Mat4* a, const Mat4* b)
{
  Mat4 result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      float sum = 0.0f;
      for (int k = 0; k < 4; k++) {
        sum += a->elements[i][k] * b->elements[k][j];
      }
      result.elements[i][j] = sum;
    }
  }
  return result;
}

Mat4 mat4_scalar_mul(const Mat4* a, float c)
{
  Mat4 result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result.elements[i][j] = a->elements[i][j] * c;
    }
  }
  return result;
}

Vec4 mat4_vec_mul(const Mat4* a, const Vec4* v)
{
  Vec4 result;
  for (int i = 0; i < 4; i++) {
    float sum = 0.0f;
    for (int j = 0; j < 4; j++) {
      sum += a->elements[i][j] * v->elements[j];
    }
    result.elements[i] = sum;
  }
  return result;
}
