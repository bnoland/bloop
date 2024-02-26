#ifndef SIMPLE_EFFECT_H_
#define SIMPLE_EFFECT_H_

#include "vector.h"
#include "matrix.h"
#include "graphics.h"

#include <stddef.h>

typedef struct
{
  Vec3 pos;
} SimpleEffectVertex;

typedef struct
{
  Vec4 pos;
} SimpleEffectVSOut;

typedef struct
{
  Vec4 pos;
} SimpleEffectGSOut;

SimpleEffectGSOut simple_effect_gsout_add(const SimpleEffectGSOut* v, const SimpleEffectGSOut* w);
SimpleEffectGSOut simple_effect_gsout_sub(const SimpleEffectGSOut* v, const SimpleEffectGSOut* w);
SimpleEffectGSOut simple_effect_gsout_mul(const SimpleEffectGSOut* v, float c);
SimpleEffectGSOut simple_effect_gsout_mul_add(const SimpleEffectGSOut* v, const SimpleEffectGSOut* w, float c);
SimpleEffectGSOut simple_effect_gsout_interpolate(const SimpleEffectGSOut* v, const SimpleEffectGSOut* w, float alpha);

typedef struct
{
  Mat4 transform;
} SimpleEffect;

SimpleEffect simple_effect_make(void);
void simple_effect_bind_transform(SimpleEffect* effect, const Mat4* transform);
void simple_effect_vertex_shader(const SimpleEffect* effect, const SimpleEffectVertex* in, SimpleEffectVSOut* out);
void simple_effect_geometry_shader(const SimpleEffect* effect,
                                   const SimpleEffectVSOut* in0,
                                   const SimpleEffectVSOut* in1,
                                   const SimpleEffectVSOut* in2,
                                   SimpleEffectGSOut* out0,
                                   SimpleEffectGSOut* out1,
                                   SimpleEffectGSOut* out2,
                                   size_t triangle_index);
Color simple_effect_pixel_shader(const SimpleEffect* effect, const SimpleEffectGSOut* in);

#endif
