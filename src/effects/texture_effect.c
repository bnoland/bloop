#include "texture_effect.h"

TextureEffectGSOut texture_effect_gsout_add(const TextureEffectGSOut* v, const TextureEffectGSOut* w)
{
  return (TextureEffectGSOut){
    .pos = vec4_add(&v->pos, &w->pos),
    .uv = vec2_add(&v->uv, &w->uv),
  };
}

TextureEffectGSOut texture_effect_gsout_sub(const TextureEffectGSOut* v, const TextureEffectGSOut* w)
{
  return (TextureEffectGSOut){
    .pos = vec4_sub(&v->pos, &w->pos),
    .uv = vec2_sub(&v->uv, &w->uv),
  };
}

TextureEffectGSOut texture_effect_gsout_mul(const TextureEffectGSOut* v, float c)
{
  return (TextureEffectGSOut){
    .pos = vec4_mul(&v->pos, c),
    .uv = vec2_mul(&v->uv, c),
  };
}

TextureEffectGSOut texture_effect_gsout_mul_add(const TextureEffectGSOut* v, const TextureEffectGSOut* w, float c)
{
  return (TextureEffectGSOut){
    .pos = vec4_mul_add(&v->pos, &w->pos, c),
    .uv = vec2_mul_add(&v->uv, &w->uv, c),
  };
}

TextureEffectGSOut texture_effect_gsout_interpolate(const TextureEffectGSOut* v,
                                                    const TextureEffectGSOut* w,
                                                    float alpha)
{
  return (TextureEffectGSOut){
    .pos = vec4_interpolate(&v->pos, &w->pos, alpha),
    .uv = vec2_interpolate(&v->uv, &w->uv, alpha),
  };
}

TextureEffect texture_effect_make(void)
{
  return (TextureEffect){
    .transform = mat4_identity(),
  };
}

void texture_effect_bind_world_view(TextureEffect* effect, const Mat4* world_view)
{
  effect->world_view = *world_view;
  effect->transform = mat4_mul(&effect->projection, &effect->world_view);
}

void texture_effect_bind_projection(TextureEffect* effect, const Mat4* projection)
{
  effect->projection = *projection;
  effect->transform = mat4_mul(&effect->projection, &effect->world_view);
}

void texture_effect_bind_texture(TextureEffect* effect, const Texture* texture)
{
  effect->texture = texture;
}

void texture_effect_vertex_shader(const TextureEffect* effect, const TextureEffectVertex* in, TextureEffectVSOut* out)
{
  const Vec4 in_pos = vec4_make(in->pos.x, in->pos.y, in->pos.z, 1.0f);
  out->pos = mat4_vec_mul(&effect->transform, &in_pos);

  out->uv = in->uv;
}

void texture_effect_geometry_shader(const TextureEffect* effect,
                                    const TextureEffectVSOut* in0,
                                    const TextureEffectVSOut* in1,
                                    const TextureEffectVSOut* in2,
                                    TextureEffectGSOut* out0,
                                    TextureEffectGSOut* out1,
                                    TextureEffectGSOut* out2,
                                    size_t triangle_index)
{
  (void)effect;
  (void)triangle_index;

  out0->pos = in0->pos;
  out1->pos = in1->pos;
  out2->pos = in2->pos;

  out0->uv = in0->uv;
  out1->uv = in1->uv;
  out2->uv = in2->uv;
}

Color texture_effect_pixel_shader(const TextureEffect* effect, const TextureEffectGSOut* in)
{
  return texture_uv_at(effect->texture, in->uv.x, in->uv.y);
}
