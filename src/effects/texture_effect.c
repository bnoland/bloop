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

TextureEffect texture_effect_make(const Graphics* graphics)
{
  return (TextureEffect){
    .graphics = graphics,
    .proj_world = mat4_identity(),
  };
}

void texture_effect_set_world(TextureEffect* effect, const Mat4* world)
{
  effect->world = *world;
  effect->proj_world = mat4_mul(&effect->projection, &effect->world);
}

void texture_effect_set_projection(TextureEffect* effect, const Mat4* projection)
{
  effect->projection = *projection;
  effect->proj_world = mat4_mul(&effect->projection, &effect->world);
}

void texture_effect_set_texture(TextureEffect* effect, const Texture* texture)
{
  effect->texture = texture;
}

void texture_effect_vertex_shader(const TextureEffect* effect, const TextureEffectVertex* in, TextureEffectVSOut* out)
{
  const Vec4 in_pos = vec4_make(in->pos.x, in->pos.y, in->pos.z, 1.0f);
  out->pos = mat4_vec_mul(&effect->proj_world, &in_pos);

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

TextureEffectGSOut texture_effect_screen_transform(const TextureEffect* effect, const TextureEffectGSOut* in)
{
  TextureEffectGSOut out;

  out.pos.x = in->pos.x / in->pos.w;
  out.pos.y = in->pos.y / in->pos.w;
  out.pos.z = in->pos.z / in->pos.w;
  out.pos.w = -1.0f / in->pos.w;

  const size_t screen_width = effect->graphics->screen_width;
  const size_t screen_height = effect->graphics->screen_height;

  out.pos.x = (screen_width / 2.0f) * (out.pos.x + 1.0f);
  out.pos.y = (screen_height / 2.0f) * (-out.pos.y + 1.0f);

  out.uv.x = in->uv.x * out.pos.w;
  out.uv.y = in->uv.y * out.pos.w;

  return out;
}

Color texture_effect_pixel_shader(const TextureEffect* effect, const TextureEffectGSOut* in)
{
  const float z = 1.0f / in->pos.w;
  const float u = in->uv.x * z;
  const float v = in->uv.y * z;
  return texture_uv_at(effect->texture, u, v);
}
