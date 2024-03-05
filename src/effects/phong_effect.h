#ifndef PHONG_EFFECT_H_
#define PHONG_EFFECT_H_

#include "vector.h"
#include "matrix.h"
#include "graphics.h"
#include "meshes/normal_mesh.h"

#include <stddef.h>

typedef NormalVertex PhongEffectVertex;

typedef struct
{
  Vec4 pos;
  Vec4 world_pos;  // Doesn't get mangled by world -> screen transformation
  Vec4 normal;
} PhongEffectVSOut;

typedef struct
{
  Vec4 pos;
  Vec4 world_pos;
  Vec4 normal;
} PhongEffectGSOut;

PhongEffectGSOut phong_effect_gsout_add(const PhongEffectGSOut* v, const PhongEffectGSOut* w);
PhongEffectGSOut phong_effect_gsout_sub(const PhongEffectGSOut* v, const PhongEffectGSOut* w);
PhongEffectGSOut phong_effect_gsout_mul(const PhongEffectGSOut* v, float c);
PhongEffectGSOut phong_effect_gsout_mul_add(const PhongEffectGSOut* v, const PhongEffectGSOut* w, float c);
PhongEffectGSOut phong_effect_gsout_interpolate(const PhongEffectGSOut* v, const PhongEffectGSOut* w, float alpha);

typedef struct
{
  const Graphics* graphics;
  Mat4 world;
  Mat4 projection;
  Mat4 proj_world;
  Vec4 light_pos;
  Vec3 ambient_light;
  Vec3 diffuse_light;
  Vec3 material_color;
  float quadratic_atten;
  float linear_atten;
  float constant_atten;
  float specular_intensity;
  float specular_power;
} PhongEffect;

PhongEffect phong_effect_make(const Graphics* graphics);

void phong_effect_set_world(PhongEffect* effect, const Mat4* world);
void phong_effect_set_projection(PhongEffect* effect, const Mat4* projection);

void phong_effect_set_light_pos(PhongEffect* effect, const Vec3* light_pos);
void phong_effect_set_diffuse_light(PhongEffect* effect, const Vec3* light);
void phong_effect_set_ambient_light(PhongEffect* effect, const Vec3* light);
void phong_effect_set_material_color(PhongEffect* effect, const Vec3* color);
void phong_effect_set_attenuation(PhongEffect* effect, float quadratic, float linear, float constant);
void phong_effect_set_specular(PhongEffect* effect, float intensity, float power);

void phong_effect_vertex_shader(const PhongEffect* effect, const PhongEffectVertex* in, PhongEffectVSOut* out);
void phong_effect_geometry_shader(const PhongEffect* effect,
                                  const PhongEffectVSOut* in0,
                                  const PhongEffectVSOut* in1,
                                  const PhongEffectVSOut* in2,
                                  PhongEffectGSOut* out0,
                                  PhongEffectGSOut* out1,
                                  PhongEffectGSOut* out2,
                                  size_t triangle_index);
PhongEffectGSOut phong_effect_screen_transform(const PhongEffect* effect, const PhongEffectGSOut* in);
Color phong_effect_pixel_shader(const PhongEffect* effect, const PhongEffectGSOut* in);

#endif
