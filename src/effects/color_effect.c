#include "color_effect.h"

ColorEffectGSOut color_effect_gsout_add(const ColorEffectGSOut* v, const ColorEffectGSOut* w)
{
  return (ColorEffectGSOut){
    .pos = vec4_add(&v->pos, &w->pos),
    .color = vec3_add(&v->color, &w->color),
  };
}

ColorEffectGSOut color_effect_gsout_sub(const ColorEffectGSOut* v, const ColorEffectGSOut* w)
{
  return (ColorEffectGSOut){
    .pos = vec4_sub(&v->pos, &w->pos),
    .color = vec3_sub(&v->color, &w->color),
  };
}

ColorEffectGSOut color_effect_gsout_mul(const ColorEffectGSOut* v, float c)
{
  return (ColorEffectGSOut){
    .pos = vec4_mul(&v->pos, c),
    .color = vec3_mul(&v->color, c),
  };
}

ColorEffectGSOut color_effect_gsout_mul_add(const ColorEffectGSOut* v, const ColorEffectGSOut* w, float c)
{
  return (ColorEffectGSOut){
    .pos = vec4_mul_add(&v->pos, &w->pos, c),
    .color = vec3_mul_add(&v->color, &w->color, c),
  };
}

ColorEffectGSOut color_effect_gsout_interpolate(const ColorEffectGSOut* v, const ColorEffectGSOut* w, float alpha)
{
  return (ColorEffectGSOut){
    .pos = vec4_interpolate(&v->pos, &w->pos, alpha),
    .color = vec3_interpolate(&v->color, &w->color, alpha),
  };
}

ColorEffect color_effect_make(void)
{
  return (ColorEffect){ .transform = mat4_identity() };
}

void color_effect_bind_transform(ColorEffect* effect, const Mat4* transform)
{
  effect->transform = *transform;
}

void color_effect_vertex_shader(const ColorEffect* effect, const ColorEffectVertex* in, ColorEffectVSOut* out)
{
  Vec4 in_pos = vec4_make(in->pos.x, in->pos.y, in->pos.z, 1.0f);
  out->pos = mat4_vec_mul(&effect->transform, &in_pos);

  out->pos.x /= out->pos.w;
  out->pos.y /= out->pos.w;
  out->pos.z /= out->pos.w;

  out->color = in->color;
}

void color_effect_geometry_shader(const ColorEffect* effect,
                                  const ColorEffectVSOut* in0,
                                  const ColorEffectVSOut* in1,
                                  const ColorEffectVSOut* in2,
                                  ColorEffectGSOut* out0,
                                  ColorEffectGSOut* out1,
                                  ColorEffectGSOut* out2,
                                  size_t triangle_index)
{
  (void)effect;
  (void)triangle_index;

  out0->pos = in0->pos;
  out1->pos = in1->pos;
  out2->pos = in2->pos;

  out0->color = in0->color;
  out1->color = in1->color;
  out2->color = in2->color;
}

Color color_effect_pixel_shader(const ColorEffect* effect, const ColorEffectGSOut* in)

{
  (void)effect;
  const Color r = in->color.x * 255.0f;
  const Color b = in->color.y * 255.0f;
  const Color g = in->color.z * 255.0f;
  const Color a = 255;
  return (r << 24) | (b << 16) | (g << 8) | a;
}
