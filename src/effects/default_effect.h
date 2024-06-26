#ifndef DEFAULT_EFFECT_H_
#define DEFAULT_EFFECT_H_

#include "vector.h"
#include "matrix.h"
#include "graphics.h"
#include "meshes/position_mesh.h"

#include <stddef.h>

typedef PositionVertex DefaultEffectVertex;

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
  const Graphics* graphics;
  Mat4 world;
  Mat4 projection;
  Mat4 proj_world;
} DefaultEffect;

DefaultEffect default_effect_make(const Graphics* graphics);

void default_effect_set_world(DefaultEffect* effect, const Mat4* world);
void default_effect_set_projection(DefaultEffect* effect, const Mat4* projection);

void default_effect_vertex_shader(const DefaultEffect* effect, const DefaultEffectVertex* in, DefaultEffectVSOut* out);
void default_effect_geometry_shader(const DefaultEffect* effect,
                                    const DefaultEffectVSOut* in0,
                                    const DefaultEffectVSOut* in1,
                                    const DefaultEffectVSOut* in2,
                                    DefaultEffectGSOut* out0,
                                    DefaultEffectGSOut* out1,
                                    DefaultEffectGSOut* out2,
                                    size_t triangle_index);
DefaultEffectGSOut default_effect_screen_transform(const DefaultEffect* effect, const DefaultEffectGSOut* in);
Color default_effect_pixel_shader(const DefaultEffect* effect, const DefaultEffectGSOut* in);

#endif
