#include "phong_effect.h"

PhongEffectGSOut phong_effect_gsout_add(const PhongEffectGSOut* v, const PhongEffectGSOut* w)
{
  return (PhongEffectGSOut){
    .pos = vec4_add(&v->pos, &w->pos),
    .normal = vec4_add(&v->normal, &w->normal),
  };
}

PhongEffectGSOut phong_effect_gsout_sub(const PhongEffectGSOut* v, const PhongEffectGSOut* w)
{
  return (PhongEffectGSOut){
    .pos = vec4_sub(&v->pos, &w->pos),
    .normal = vec4_sub(&v->normal, &w->normal),
  };
}

PhongEffectGSOut phong_effect_gsout_mul(const PhongEffectGSOut* v, float c)
{
  return (PhongEffectGSOut){
    .pos = vec4_mul(&v->pos, c),
    .normal = vec4_mul(&v->normal, c),
  };
}

PhongEffectGSOut phong_effect_gsout_mul_add(const PhongEffectGSOut* v, const PhongEffectGSOut* w, float c)
{
  return (PhongEffectGSOut){
    .pos = vec4_mul_add(&v->pos, &w->pos, c),
    .normal = vec4_mul_add(&v->normal, &w->normal, c),
  };
}

PhongEffectGSOut phong_effect_gsout_interpolate(const PhongEffectGSOut* v, const PhongEffectGSOut* w, float alpha)
{
  return (PhongEffectGSOut){
    .pos = vec4_interpolate(&v->pos, &w->pos, alpha),
    .normal = vec4_interpolate(&v->normal, &w->normal, alpha),
  };
}

PhongEffect phong_effect_make(const Graphics* graphics)
{
  return (PhongEffect){
    .graphics = graphics,
    .transform = mat4_identity(),
  };
}

void phong_effect_bind_world_view(PhongEffect* effect, const Mat4* world_view)
{
  effect->world_view = *world_view;
  effect->transform = mat4_mul(&effect->projection, &effect->world_view);
}

void phong_effect_bind_projection(PhongEffect* effect, const Mat4* projection)
{
  effect->projection = *projection;
  effect->transform = mat4_mul(&effect->projection, &effect->world_view);
}

void phong_effect_vertex_shader(const PhongEffect* effect, const PhongEffectVertex* in, PhongEffectVSOut* out)
{
  const Vec4 in_pos = vec4_make(in->pos.x, in->pos.y, in->pos.z, 1.0f);
  const Vec4 in_normal = vec4_make(in->normal.x, in->normal.y, in->normal.z, 1.0f);
  out->pos = mat4_vec_mul(&effect->transform, &in_pos);
  out->normal = mat4_vec_mul(&effect->transform, &in_normal);
}

void phong_effect_geometry_shader(const PhongEffect* effect,
                                  const PhongEffectVSOut* in0,
                                  const PhongEffectVSOut* in1,
                                  const PhongEffectVSOut* in2,
                                  PhongEffectGSOut* out0,
                                  PhongEffectGSOut* out1,
                                  PhongEffectGSOut* out2,
                                  size_t triangle_index)
{
  (void)effect;
  (void)triangle_index;
  out0->pos = in0->pos;
  out1->pos = in1->pos;
  out2->pos = in2->pos;
}

Color phong_effect_pixel_shader(const PhongEffect* effect, const PhongEffectGSOut* in)
{
  // XXX: Implement.
  (void)effect;
  (void)in;
  return 0xffffffff;
}

PhongEffectGSOut phong_effect_screen_transform(const PhongEffect* effect, const PhongEffectGSOut* in)
{
  PhongEffectGSOut out;

  out.pos.x = in->pos.x / in->pos.w;
  out.pos.y = in->pos.y / in->pos.w;
  out.pos.z = in->pos.z / in->pos.w;
  out.pos.w = -1.0f / in->pos.w;

  const size_t screen_width = effect->graphics->screen_width;
  const size_t screen_height = effect->graphics->screen_height;

  out.pos.x = (screen_width / 2.0f) * (out.pos.x + 1.0f);
  out.pos.y = (screen_height / 2.0f) * (-out.pos.y + 1.0f);

  out.normal = vec4_mul(&in->normal, out.pos.w);

  return out;
}
