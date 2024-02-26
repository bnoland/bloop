#include "rainbow_effect.h"

RainbowEffectGSOut rainbow_effect_gsout_add(const RainbowEffectGSOut* v, const RainbowEffectGSOut* w)
{
  return (RainbowEffectGSOut){
    .pos = vec4_add(&v->pos, &w->pos),
    .color = vec3_add(&v->color, &w->color),
  };
}

RainbowEffectGSOut rainbow_effect_gsout_sub(const RainbowEffectGSOut* v, const RainbowEffectGSOut* w)
{
  return (RainbowEffectGSOut){
    .pos = vec4_sub(&v->pos, &w->pos),
    .color = vec3_sub(&v->color, &w->color),
  };
}

RainbowEffectGSOut rainbow_effect_gsout_mul(const RainbowEffectGSOut* v, float c)
{
  return (RainbowEffectGSOut){
    .pos = vec4_mul(&v->pos, c),
    .color = vec3_mul(&v->color, c),
  };
}

RainbowEffectGSOut rainbow_effect_gsout_mul_add(const RainbowEffectGSOut* v, const RainbowEffectGSOut* w, float c)
{
  return (RainbowEffectGSOut){
    .pos = vec4_mul_add(&v->pos, &w->pos, c),
    .color = vec3_mul_add(&v->color, &w->color, c),
  };
}

RainbowEffectGSOut rainbow_effect_gsout_interpolate(const RainbowEffectGSOut* v,
                                                    const RainbowEffectGSOut* w,
                                                    float alpha)
{
  return (RainbowEffectGSOut){
    .pos = vec4_interpolate(&v->pos, &w->pos, alpha),
    .color = vec3_interpolate(&v->color, &w->color, alpha),
  };
}

RainbowEffect rainbow_effect_make(void)
{
  return (RainbowEffect){ .transform = mat4_identity() };
}

void rainbow_effect_bind_transform(RainbowEffect* effect, const Mat4* transform)
{
  effect->transform = *transform;
}

void rainbow_effect_vertex_shader(const RainbowEffect* effect, const RainbowEffectVertex* in, RainbowEffectVSOut* out)
{
  Vec4 in_pos = vec4_make(in->pos.x, in->pos.y, in->pos.z, 1.0f);
  out->pos = mat4_vec_mul(&effect->transform, &in_pos);

  out->pos.x /= -out->pos.z;
  out->pos.y /= -out->pos.z;

  out->color = in->color;
}

void rainbow_effect_geometry_shader(const RainbowEffect* effect,
                                    const RainbowEffectVSOut* in0,
                                    const RainbowEffectVSOut* in1,
                                    const RainbowEffectVSOut* in2,
                                    RainbowEffectGSOut* out0,
                                    RainbowEffectGSOut* out1,
                                    RainbowEffectGSOut* out2,
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

Color rainbow_effect_pixel_shader(const RainbowEffect* effect, const RainbowEffectGSOut* in)

{
  (void)effect;
  const Color r = in->color.x * 255.0f;
  const Color b = in->color.y * 255.0f;
  const Color g = in->color.z * 255.0f;
  const Color a = 255;
  return (r << 24) | (b << 16) | (g << 8) | a;
}
