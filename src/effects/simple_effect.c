#include "simple_effect.h"

SimpleEffectGSOut simple_effect_gsout_add(const SimpleEffectGSOut* v, const SimpleEffectGSOut* w)
{
  return (SimpleEffectGSOut){ .pos = vec4_add(&v->pos, &w->pos) };
}

SimpleEffectGSOut simple_effect_gsout_sub(const SimpleEffectGSOut* v, const SimpleEffectGSOut* w)
{
  return (SimpleEffectGSOut){ .pos = vec4_sub(&v->pos, &w->pos) };
}

SimpleEffectGSOut simple_effect_gsout_mul(const SimpleEffectGSOut* v, float c)
{
  return (SimpleEffectGSOut){ .pos = vec4_mul(&v->pos, c) };
}

SimpleEffectGSOut simple_effect_gsout_mul_add(const SimpleEffectGSOut* v, const SimpleEffectGSOut* w, float c)
{
  return (SimpleEffectGSOut){ .pos = vec4_mul_add(&v->pos, &w->pos, c) };
}

SimpleEffectGSOut simple_effect_gsout_interpolate(const SimpleEffectGSOut* v, const SimpleEffectGSOut* w, float alpha)
{
  return (SimpleEffectGSOut){ .pos = vec4_interpolate(&v->pos, &w->pos, alpha) };
}

SimpleEffect simple_effect_make(void)
{
  return (SimpleEffect){ .transform = mat4_identity() };
}

void simple_effect_bind_transform(SimpleEffect* effect, const Mat4* transform)
{
  effect->transform = *transform;
}

void simple_effect_vertex_shader(const SimpleEffect* effect, const SimpleEffectVertex* in, SimpleEffectVSOut* out)
{
  Vec4 in_pos = vec4_make(in->pos.x, in->pos.y, in->pos.z, 1.0f);
  out->pos = mat4_vec_mul(&effect->transform, &in_pos);
  out->pos.x /= -out->pos.z;
  out->pos.y /= -out->pos.z;
}

void simple_effect_geometry_shader(const SimpleEffect* effect,
                                   const SimpleEffectVSOut* in0,
                                   const SimpleEffectVSOut* in1,
                                   const SimpleEffectVSOut* in2,
                                   SimpleEffectGSOut* out0,
                                   SimpleEffectGSOut* out1,
                                   SimpleEffectGSOut* out2,
                                   size_t triangle_index)
{
  (void)effect;
  (void)triangle_index;
  out0->pos = in0->pos;
  out1->pos = in1->pos;
  out2->pos = in2->pos;
}

Color simple_effect_pixel_shader(const SimpleEffect* effect, const SimpleEffectGSOut* in)

{
  (void)effect;
  (void)in;
  return 0xffffffff;
}
