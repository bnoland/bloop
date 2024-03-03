#ifndef TEXTURE_EFFECT_H_
#define TEXTURE_EFFECT_H_

#include "vector.h"
#include "matrix.h"
#include "graphics.h"
#include "texture.h"

#include <stddef.h>

typedef struct
{
  Vec3 pos;
  Vec2 uv;
} TextureEffectVertex;

typedef struct
{
  Vec4 pos;
  Vec2 uv;
} TextureEffectVSOut;

typedef struct
{
  Vec4 pos;
  Vec2 uv;
} TextureEffectGSOut;

TextureEffectGSOut texture_effect_gsout_add(const TextureEffectGSOut* v, const TextureEffectGSOut* w);
TextureEffectGSOut texture_effect_gsout_sub(const TextureEffectGSOut* v, const TextureEffectGSOut* w);
TextureEffectGSOut texture_effect_gsout_mul(const TextureEffectGSOut* v, float c);
TextureEffectGSOut texture_effect_gsout_mul_add(const TextureEffectGSOut* v, const TextureEffectGSOut* w, float c);
TextureEffectGSOut texture_effect_gsout_interpolate(const TextureEffectGSOut* v,
                                                    const TextureEffectGSOut* w,
                                                    float alpha);

typedef struct
{
  Mat4 world_view;
  Mat4 projection;
  Mat4 transform;
  const Texture* texture;
  const Graphics* graphics;
} TextureEffect;

TextureEffect texture_effect_make(const Graphics* graphics);
void texture_effect_bind_world_view(TextureEffect* effect, const Mat4* world_view);
void texture_effect_bind_projection(TextureEffect* effect, const Mat4* projection);
void texture_effect_bind_texture(TextureEffect* effect, const Texture* texture);
void texture_effect_vertex_shader(const TextureEffect* effect, const TextureEffectVertex* in, TextureEffectVSOut* out);
void texture_effect_geometry_shader(const TextureEffect* effect,
                                    const TextureEffectVSOut* in0,
                                    const TextureEffectVSOut* in1,
                                    const TextureEffectVSOut* in2,
                                    TextureEffectGSOut* out0,
                                    TextureEffectGSOut* out1,
                                    TextureEffectGSOut* out2,
                                    size_t triangle_index);
Color texture_effect_pixel_shader(const TextureEffect* effect, const TextureEffectGSOut* in);
TextureEffectGSOut texture_effect_screen_transform(const TextureEffect* effect, const TextureEffectGSOut* in);

#endif
