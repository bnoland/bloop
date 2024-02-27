#include "default_effect.h"

DefaultEffectGSOut default_effect_gsout_add(const DefaultEffectGSOut* v, const DefaultEffectGSOut* w)
{
  return (DefaultEffectGSOut){ .pos = vec4_add(&v->pos, &w->pos) };
}

DefaultEffectGSOut default_effect_gsout_sub(const DefaultEffectGSOut* v, const DefaultEffectGSOut* w)
{
  return (DefaultEffectGSOut){ .pos = vec4_sub(&v->pos, &w->pos) };
}

DefaultEffectGSOut default_effect_gsout_mul(const DefaultEffectGSOut* v, float c)
{
  return (DefaultEffectGSOut){ .pos = vec4_mul(&v->pos, c) };
}

DefaultEffectGSOut default_effect_gsout_mul_add(const DefaultEffectGSOut* v, const DefaultEffectGSOut* w, float c)
{
  return (DefaultEffectGSOut){ .pos = vec4_mul_add(&v->pos, &w->pos, c) };
}

DefaultEffectGSOut default_effect_gsout_interpolate(const DefaultEffectGSOut* v,
                                                    const DefaultEffectGSOut* w,
                                                    float alpha)
{
  return (DefaultEffectGSOut){ .pos = vec4_interpolate(&v->pos, &w->pos, alpha) };
}

DefaultEffect default_effect_make(void)
{
  return (DefaultEffect){ .transform = mat4_identity() };
}

void default_effect_bind_transform(DefaultEffect* effect, const Mat4* transform)
{
  effect->transform = *transform;
}

void default_effect_vertex_shader(const DefaultEffect* effect, const DefaultEffectVertex* in, DefaultEffectVSOut* out)
{
  Vec4 in_pos = vec4_make(in->pos.x, in->pos.y, in->pos.z, 1.0f);
  out->pos = mat4_vec_mul(&effect->transform, &in_pos);
  out->pos.x /= -out->pos.z;
  out->pos.y /= -out->pos.z;
}

void default_effect_geometry_shader(const DefaultEffect* effect,
                                    const DefaultEffectVSOut* in0,
                                    const DefaultEffectVSOut* in1,
                                    const DefaultEffectVSOut* in2,
                                    DefaultEffectGSOut* out0,
                                    DefaultEffectGSOut* out1,
                                    DefaultEffectGSOut* out2,
                                    size_t triangle_index)
{
  (void)effect;
  (void)triangle_index;
  out0->pos = in0->pos;
  out1->pos = in1->pos;
  out2->pos = in2->pos;
}

Color default_effect_pixel_shader(const DefaultEffect* effect, const DefaultEffectGSOut* in)

{
  (void)effect;
  (void)in;
  return 0xffffffff;
}
