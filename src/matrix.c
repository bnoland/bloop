#include "matrix.h"

#include "utility.h"

#include <tgmath.h>

void mat3_zero(Mat3* dest)
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      dest->elements[i][j] = 0.0f;
    }
  }
}

void mat3_identity(Mat3* dest)
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      dest->elements[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
}

void mat3_add(Mat3* dest, const Mat3* a, const Mat3* b)
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      dest->elements[i][j] = a->elements[i][j] + b->elements[i][j];
    }
  }
}

void mat3_sub(Mat3* dest, const Mat3* a, const Mat3* b)
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      dest->elements[i][j] = a->elements[i][j] - b->elements[i][j];
    }
  }
}

void mat3_mul(Mat3* dest, const Mat3* a, const Mat3* b)
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      float sum = 0.0f;
      for (int k = 0; k < 3; k++) {
        sum += a->elements[i][k] * b->elements[k][j];
      }
      dest->elements[i][j] = sum;
    }
  }
}

void mat3_scalar_mul(Mat3* dest, const Mat3* a, float c)
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      dest->elements[i][j] = a->elements[i][j] * c;
    }
  }
}

void mat3_vec_mul(Vec3* dest, const Mat3* a, const Vec3* v)
{
  for (int i = 0; i < 3; i++) {
    float sum = 0.0f;
    for (int j = 0; j < 3; j++) {
      sum += a->elements[i][j] * v->elements[j];
    }
    dest->elements[i] = sum;
  }
}

void mat4_zero(Mat4* dest)
{
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dest->elements[i][j] = 0.0f;
    }
  }
}

void mat4_identity(Mat4* dest)
{
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dest->elements[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
}

void mat4_projection(Mat4* dest, float fov, float aspect_ratio, float near, float far)
{
  mat4_zero(dest);

  const float fov_radians = degrees_to_radians(fov);
  const float width = -2.0f * near * tanf(fov_radians / 2.0f);
  const float height = aspect_ratio * width;

  dest->elements[0][0] = 2.0f * near / width;
  dest->elements[1][1] = 2.0f * near / height;
  dest->elements[2][2] = (far + near) / (far - near);
  dest->elements[2][3] = -2.0f * near * far / (far - near);
  dest->elements[3][2] = 1.0f;
}

void mat4_rotation_x(Mat4* dest, float angle)
{
  mat4_identity(dest);

  const double cos_angle = cos(angle);
  const double sin_angle = sin(angle);

  dest->elements[1][1] = cos_angle;
  dest->elements[1][2] = -sin_angle;
  dest->elements[2][1] = sin_angle;
  dest->elements[2][2] = cos_angle;
}

void mat4_rotation_y(Mat4* dest, float angle)
{
  mat4_identity(dest);

  const double cos_angle = cos(angle);
  const double sin_angle = sin(angle);

  dest->elements[0][0] = cos_angle;
  dest->elements[0][3] = sin_angle;
  dest->elements[2][0] = -sin_angle;
  dest->elements[2][2] = cos_angle;
}

void mat4_rotation_z(Mat4* dest, float angle)
{
  mat4_identity(dest);

  const double cos_angle = cos(angle);
  const double sin_angle = sin(angle);

  dest->elements[0][0] = cos_angle;
  dest->elements[0][1] = -sin_angle;
  dest->elements[1][0] = sin_angle;
  dest->elements[1][1] = cos_angle;
}

void mat4_translation(Mat4* dest, float x, float y, float z)
{
  mat4_identity(dest);

  dest->elements[0][3] = x;
  dest->elements[1][3] = y;
  dest->elements[2][3] = z;
}

void mat4_add(Mat4* dest, const Mat4* a, const Mat4* b)
{
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dest->elements[i][j] = a->elements[i][j] + b->elements[i][j];
    }
  }
}

void mat4_sub(Mat4* dest, const Mat4* a, const Mat4* b)
{
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dest->elements[i][j] = a->elements[i][j] - b->elements[i][j];
    }
  }
}

void mat4_mul(Mat4* dest, const Mat4* a, const Mat4* b)
{
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      float sum = 0.0f;
      for (int k = 0; k < 4; k++) {
        sum += a->elements[i][k] * b->elements[k][j];
      }
      dest->elements[i][j] = sum;
    }
  }
}

void mat4_scalar_mul(Mat4* dest, const Mat4* a, float c)
{
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dest->elements[i][j] = a->elements[i][j] * c;
    }
  }
}

void mat4_vec_mul(Vec4* dest, const Mat4* a, const Vec4* v)
{
  for (int i = 0; i < 4; i++) {
    float sum = 0.0f;
    for (int j = 0; j < 4; j++) {
      sum += a->elements[i][j] * v->elements[j];
    }
    dest->elements[i] = sum;
  }
}
