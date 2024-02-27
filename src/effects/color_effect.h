#ifndef COLOR_EFFECT_H_
#define COLOR_EFFECT_H_

#include "vector.h"
#include "matrix.h"
#include "graphics.h"

#include <stddef.h>

typedef struct
{
  Vec3 pos;
  Vec3 color;
} ColorEffectVertex;

typedef struct
{
  Vec4 pos;
  Vec3 color;
} ColorEffectVSOut;

typedef struct
{
  Vec4 pos;
  Vec3 color;
} ColorEffectGSOut;

ColorEffectGSOut color_effect_gsout_add(const ColorEffectGSOut* v, const ColorEffectGSOut* w);
ColorEffectGSOut color_effect_gsout_sub(const ColorEffectGSOut* v, const ColorEffectGSOut* w);
ColorEffectGSOut color_effect_gsout_mul(const ColorEffectGSOut* v, float c);
ColorEffectGSOut color_effect_gsout_mul_add(const ColorEffectGSOut* v, const ColorEffectGSOut* w, float c);
ColorEffectGSOut color_effect_gsout_interpolate(const ColorEffectGSOut* v, const ColorEffectGSOut* w, float alpha);

typedef struct
{
  Mat4 transform;
} ColorEffect;

ColorEffect color_effect_make(void);
void color_effect_bind_transform(ColorEffect* effect, const Mat4* transform);
void color_effect_vertex_shader(const ColorEffect* effect, const ColorEffectVertex* in, ColorEffectVSOut* out);
void color_effect_geometry_shader(const ColorEffect* effect,
                                  const ColorEffectVSOut* in0,
                                  const ColorEffectVSOut* in1,
                                  const ColorEffectVSOut* in2,
                                  ColorEffectGSOut* out0,
                                  ColorEffectGSOut* out1,
                                  ColorEffectGSOut* out2,
                                  size_t triangle_index);
Color color_effect_pixel_shader(const ColorEffect* effect, const ColorEffectGSOut* in);

#endif
