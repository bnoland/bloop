#ifndef GOURAUD_EFFECT_H_
#define GOURAUD_EFFECT_H_

#include "vector.h"
#include "matrix.h"
#include "graphics.h"

#include <stddef.h>

typedef struct
{
  Vec3 pos;
  Vec3 normal;
} GouraudEffectVertex;

typedef struct
{
  Vec4 pos;
  Vec3 color;
} GouraudEffectVSOut;

typedef struct
{
  Vec4 pos;
  Vec3 color;
} GouraudEffectGSOut;

GouraudEffectGSOut gouraud_effect_gsout_add(const GouraudEffectGSOut* v, const GouraudEffectGSOut* w);
GouraudEffectGSOut gouraud_effect_gsout_sub(const GouraudEffectGSOut* v, const GouraudEffectGSOut* w);
GouraudEffectGSOut gouraud_effect_gsout_mul(const GouraudEffectGSOut* v, float c);
GouraudEffectGSOut gouraud_effect_gsout_mul_add(const GouraudEffectGSOut* v, const GouraudEffectGSOut* w, float c);
GouraudEffectGSOut gouraud_effect_gsout_interpolate(const GouraudEffectGSOut* v,
                                                    const GouraudEffectGSOut* w,
                                                    float alpha);

typedef struct
{
  Mat4 world_view;
  Mat4 projection;
  Mat4 transform;
} GouraudEffect;

GouraudEffect gouraud_effect_make(void);
void gouraud_effect_bind_world_view(GouraudEffect* effect, const Mat4* world_view);
void gouraud_effect_bind_projection(GouraudEffect* effect, const Mat4* projection);
void gouraud_effect_vertex_shader(const GouraudEffect* effect, const GouraudEffectVertex* in, GouraudEffectVSOut* out);
void gouraud_effect_geometry_shader(const GouraudEffect* effect,
                                    const GouraudEffectVSOut* in0,
                                    const GouraudEffectVSOut* in1,
                                    const GouraudEffectVSOut* in2,
                                    GouraudEffectGSOut* out0,
                                    GouraudEffectGSOut* out1,
                                    GouraudEffectGSOut* out2,
                                    size_t triangle_index);
Color gouraud_effect_pixel_shader(const GouraudEffect* effect, const GouraudEffectGSOut* in);

#endif
