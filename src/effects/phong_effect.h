#ifndef PHONG_EFFECT_H_
#define PHONG_EFFECT_H_

#include "vector.h"
#include "matrix.h"
#include "graphics.h"
#include "meshes/normal_mesh.h"

#include <stddef.h>

typedef NormalVertex PhongEffectVertex;

typedef struct
{
  Vec4 pos;
  Vec4 normal;
} PhongEffectVSOut;

typedef struct
{
  Vec4 pos;
  Vec4 normal;
} PhongEffectGSOut;

PhongEffectGSOut phong_effect_gsout_add(const PhongEffectGSOut* v, const PhongEffectGSOut* w);
PhongEffectGSOut phong_effect_gsout_sub(const PhongEffectGSOut* v, const PhongEffectGSOut* w);
PhongEffectGSOut phong_effect_gsout_mul(const PhongEffectGSOut* v, float c);
PhongEffectGSOut phong_effect_gsout_mul_add(const PhongEffectGSOut* v, const PhongEffectGSOut* w, float c);
PhongEffectGSOut phong_effect_gsout_interpolate(const PhongEffectGSOut* v, const PhongEffectGSOut* w, float alpha);

typedef struct
{
  Mat4 world_view;
  Mat4 projection;
  Mat4 transform;
  const Graphics* graphics;
} PhongEffect;

PhongEffect phong_effect_make(const Graphics* graphics);
void phong_effect_bind_world_view(PhongEffect* effect, const Mat4* world_view);
void phong_effect_bind_projection(PhongEffect* effect, const Mat4* projection);
void phong_effect_vertex_shader(const PhongEffect* effect, const PhongEffectVertex* in, PhongEffectVSOut* out);
void phong_effect_geometry_shader(const PhongEffect* effect,
                                  const PhongEffectVSOut* in0,
                                  const PhongEffectVSOut* in1,
                                  const PhongEffectVSOut* in2,
                                  PhongEffectGSOut* out0,
                                  PhongEffectGSOut* out1,
                                  PhongEffectGSOut* out2,
                                  size_t triangle_index);
Color phong_effect_pixel_shader(const PhongEffect* effect, const PhongEffectGSOut* in);
PhongEffectGSOut phong_effect_screen_transform(const PhongEffect* effect, const PhongEffectGSOut* in);

#endif
