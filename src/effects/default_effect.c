#include "default_effect.h"

DefaultEffectGSOut default_effect_gsout_add(const DefaultEffectGSOut* v, const DefaultEffectGSOut* w)
{
  return (DefaultEffectGSOut){
    .pos = vec4_add(&v->pos, &w->pos),
  };
}

DefaultEffectGSOut default_effect_gsout_sub(const DefaultEffectGSOut* v, const DefaultEffectGSOut* w)
{
  return (DefaultEffectGSOut){
    .pos = vec4_sub(&v->pos, &w->pos),
  };
}

DefaultEffectGSOut default_effect_gsout_mul(const DefaultEffectGSOut* v, float c)
{
  return (DefaultEffectGSOut){
    .pos = vec4_mul(&v->pos, c),
  };
}

DefaultEffectGSOut default_effect_gsout_mul_add(const DefaultEffectGSOut* v, const DefaultEffectGSOut* w, float c)
{
  return (DefaultEffectGSOut){
    .pos = vec4_mul_add(&v->pos, &w->pos, c),
  };
}

DefaultEffectGSOut default_effect_gsout_interpolate(const DefaultEffectGSOut* v,
                                                    const DefaultEffectGSOut* w,
                                                    float alpha)
{
  return (DefaultEffectGSOut){
    .pos = vec4_interpolate(&v->pos, &w->pos, alpha),
  };
}

DefaultEffect default_effect_make(const Graphics* graphics)
{
  return (DefaultEffect){
    .graphics = graphics,
    .transform = mat4_identity(),
  };
}

void default_effect_bind_world_view(DefaultEffect* effect, const Mat4* world_view)
{
  effect->world_view = *world_view;
  effect->transform = mat4_mul(&effect->projection, &effect->world_view);
}

void default_effect_bind_projection(DefaultEffect* effect, const Mat4* projection)
{
  effect->projection = *projection;
  effect->transform = mat4_mul(&effect->projection, &effect->world_view);
}

void default_effect_vertex_shader(const DefaultEffect* effect, const DefaultEffectVertex* in, DefaultEffectVSOut* out)
{
  const Vec4 in_pos = vec4_make(in->pos.x, in->pos.y, in->pos.z, 1.0f);
  out->pos = mat4_vec_mul(&effect->transform, &in_pos);
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

DefaultEffectGSOut default_effect_screen_transform(const DefaultEffect* effect, const DefaultEffectGSOut* in)
{
  DefaultEffectGSOut out;

  out.pos.x = in->pos.x / in->pos.w;
  out.pos.y = in->pos.y / in->pos.w;
  out.pos.z = in->pos.z / in->pos.w;
  out.pos.w = 1.0f;

  const size_t screen_width = effect->graphics->screen_width;
  const size_t screen_height = effect->graphics->screen_height;

  out.pos.x = (screen_width / 2.0f) * (out.pos.x + 1.0f);
  out.pos.y = (screen_height / 2.0f) * (-out.pos.y + 1.0f);

  return out;
}
