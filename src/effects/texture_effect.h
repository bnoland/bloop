#ifndef TEXTURE_EFFECT_H_
#define TEXTURE_EFFECT_H_

#include "vector.h"
#include "matrix.h"
#include "graphics.h"
#include "texture.h"
#include "meshes/texture_mesh.h"

#include <stddef.h>

typedef TextureVertex TextureEffectVertex;

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
  const Texture* texture;
  const Graphics* graphics;
  Mat4 world;
  Mat4 projection;
  Mat4 proj_world;
} TextureEffect;

TextureEffect texture_effect_make(const Graphics* graphics);

void texture_effect_set_world(TextureEffect* effect, const Mat4* world);
void texture_effect_set_projection(TextureEffect* effect, const Mat4* projection);
void texture_effect_set_texture(TextureEffect* effect, const Texture* texture);

void texture_effect_vertex_shader(const TextureEffect* effect, const TextureEffectVertex* in, TextureEffectVSOut* out);
void texture_effect_geometry_shader(const TextureEffect* effect,
                                    const TextureEffectVSOut* in0,
                                    const TextureEffectVSOut* in1,
                                    const TextureEffectVSOut* in2,
                                    TextureEffectGSOut* out0,
                                    TextureEffectGSOut* out1,
                                    TextureEffectGSOut* out2,
                                    size_t triangle_index);
TextureEffectGSOut texture_effect_screen_transform(const TextureEffect* effect, const TextureEffectGSOut* in);
Color texture_effect_pixel_shader(const TextureEffect* effect, const TextureEffectGSOut* in);

#endif
