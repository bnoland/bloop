#ifndef RAINBOW_EFFECT_H_
#define RAINBOW_EFFECT_H_

#include "vector.h"
#include "matrix.h"
#include "graphics.h"

#include <stddef.h>

typedef struct
{
  Vec3 pos;
  Vec3 color;
} RainbowEffectVertex;

typedef struct
{
  Vec4 pos;
  Vec3 color;
} RainbowEffectVSOut;

typedef struct
{
  Vec4 pos;
  Vec3 color;
} RainbowEffectGSOut;

RainbowEffectGSOut rainbow_effect_gsout_add(const RainbowEffectGSOut* v, const RainbowEffectGSOut* w);
RainbowEffectGSOut rainbow_effect_gsout_sub(const RainbowEffectGSOut* v, const RainbowEffectGSOut* w);
RainbowEffectGSOut rainbow_effect_gsout_mul(const RainbowEffectGSOut* v, float c);
RainbowEffectGSOut rainbow_effect_gsout_mul_add(const RainbowEffectGSOut* v, const RainbowEffectGSOut* w, float c);
RainbowEffectGSOut rainbow_effect_gsout_interpolate(const RainbowEffectGSOut* v,
                                                    const RainbowEffectGSOut* w,
                                                    float alpha);

typedef struct
{
  Mat4 transform;
} RainbowEffect;

RainbowEffect rainbow_effect_make(void);
void rainbow_effect_bind_transform(RainbowEffect* effect, const Mat4* transform);
void rainbow_effect_vertex_shader(const RainbowEffect* effect, const RainbowEffectVertex* in, RainbowEffectVSOut* out);
void rainbow_effect_geometry_shader(const RainbowEffect* effect,
                                    const RainbowEffectVSOut* in0,
                                    const RainbowEffectVSOut* in1,
                                    const RainbowEffectVSOut* in2,
                                    RainbowEffectGSOut* out0,
                                    RainbowEffectGSOut* out1,
                                    RainbowEffectGSOut* out2,
                                    size_t triangle_index);
Color rainbow_effect_pixel_shader(const RainbowEffect* effect, const RainbowEffectGSOut* in);

#endif
