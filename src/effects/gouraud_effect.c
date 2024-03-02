#include "gouraud_effect.h"

GouraudEffectGSOut gouraud_effect_gsout_add(const GouraudEffectGSOut* v, const GouraudEffectGSOut* w)
{
  return (GouraudEffectGSOut){ .pos = vec4_add(&v->pos, &w->pos) };
}

GouraudEffectGSOut gouraud_effect_gsout_sub(const GouraudEffectGSOut* v, const GouraudEffectGSOut* w)
{
  return (GouraudEffectGSOut){ .pos = vec4_sub(&v->pos, &w->pos) };
}

GouraudEffectGSOut gouraud_effect_gsout_mul(const GouraudEffectGSOut* v, float c)
{
  return (GouraudEffectGSOut){ .pos = vec4_mul(&v->pos, c) };
}

GouraudEffectGSOut gouraud_effect_gsout_mul_add(const GouraudEffectGSOut* v, const GouraudEffectGSOut* w, float c)
{
  return (GouraudEffectGSOut){ .pos = vec4_mul_add(&v->pos, &w->pos, c) };
}

GouraudEffectGSOut gouraud_effect_gsout_interpolate(const GouraudEffectGSOut* v,
                                                    const GouraudEffectGSOut* w,
                                                    float alpha)
{
  return (GouraudEffectGSOut){ .pos = vec4_interpolate(&v->pos, &w->pos, alpha) };
}

GouraudEffect gouraud_effect_make(void)
{
  return (GouraudEffect){ .world_view = mat4_identity() };
}

void gouraud_effect_bind_world_view(GouraudEffect* effect, const Mat4* world_view)
{
  effect->world_view = *world_view;
  effect->transform = mat4_mul(&effect->projection, &effect->world_view);
}

void gouraud_effect_bind_projection(GouraudEffect* effect, const Mat4* projection)
{
  effect->projection = *projection;
  effect->transform = mat4_mul(&effect->projection, &effect->world_view);
}

void gouraud_effect_vertex_shader(const GouraudEffect* effect, const GouraudEffectVertex* in, GouraudEffectVSOut* out)
{
  // XXX: Implement.
  Vec4 in_pos = vec4_make(in->pos.x, in->pos.y, in->pos.z, 1.0f);
  out->pos = mat4_vec_mul(&effect->transform, &in_pos);
}

void gouraud_effect_geometry_shader(const GouraudEffect* effect,
                                    const GouraudEffectVSOut* in0,
                                    const GouraudEffectVSOut* in1,
                                    const GouraudEffectVSOut* in2,
                                    GouraudEffectGSOut* out0,
                                    GouraudEffectGSOut* out1,
                                    GouraudEffectGSOut* out2,
                                    size_t triangle_index)
{
  (void)effect;
  (void)triangle_index;
  out0->pos = in0->pos;
  out1->pos = in1->pos;
  out2->pos = in2->pos;
}

Color gouraud_effect_pixel_shader(const GouraudEffect* effect, const GouraudEffectGSOut* in)

{
  // XXX: Implement.
  (void)effect;
  (void)in;
  return 0xffffffff;
}
