#ifndef DEFAULT_EFFECT_H_
#define DEFAULT_EFFECT_H_

#include "vector.h"
#include "matrix.h"
#include "graphics.h"

#include <stddef.h>

typedef struct
{
  Vec3 pos;
} DefaultEffectVertex;

typedef struct
{
  Vec4 pos;
} DefaultEffectVSOut;

typedef struct
{
  Vec4 pos;
} DefaultEffectGSOut;

DefaultEffectGSOut default_effect_gsout_add(const DefaultEffectGSOut* v, const DefaultEffectGSOut* w);
DefaultEffectGSOut default_effect_gsout_sub(const DefaultEffectGSOut* v, const DefaultEffectGSOut* w);
DefaultEffectGSOut default_effect_gsout_mul(const DefaultEffectGSOut* v, float c);
DefaultEffectGSOut default_effect_gsout_mul_add(const DefaultEffectGSOut* v, const DefaultEffectGSOut* w, float c);
DefaultEffectGSOut default_effect_gsout_interpolate(const DefaultEffectGSOut* v,
                                                    const DefaultEffectGSOut* w,
                                                    float alpha);

typedef struct
{
  Mat4 transform;
} DefaultEffect;

DefaultEffect default_effect_make(void);
void default_effect_bind_transform(DefaultEffect* effect, const Mat4* transform);
void default_effect_vertex_shader(const DefaultEffect* effect, const DefaultEffectVertex* in, DefaultEffectVSOut* out);
void default_effect_geometry_shader(const DefaultEffect* effect,
                                    const DefaultEffectVSOut* in0,
                                    const DefaultEffectVSOut* in1,
                                    const DefaultEffectVSOut* in2,
                                    DefaultEffectGSOut* out0,
                                    DefaultEffectGSOut* out1,
                                    DefaultEffectGSOut* out2,
                                    size_t triangle_index);
Color default_effect_pixel_shader(const DefaultEffect* effect, const DefaultEffectGSOut* in);

#endif
