#include "phong_effect.h"

#include <tgmath.h>

PhongEffectGSOut phong_effect_gsout_add(const PhongEffectGSOut* v, const PhongEffectGSOut* w)
{
  return (PhongEffectGSOut){
    .pos = vec4_add(&v->pos, &w->pos),
    .world_pos = vec4_add(&v->world_pos, &w->world_pos),
    .normal = vec4_add(&v->normal, &w->normal),
  };
}

PhongEffectGSOut phong_effect_gsout_sub(const PhongEffectGSOut* v, const PhongEffectGSOut* w)
{
  return (PhongEffectGSOut){
    .pos = vec4_sub(&v->pos, &w->pos),
    .world_pos = vec4_sub(&v->world_pos, &w->world_pos),
    .normal = vec4_sub(&v->normal, &w->normal),
  };
}

PhongEffectGSOut phong_effect_gsout_mul(const PhongEffectGSOut* v, float c)
{
  return (PhongEffectGSOut){
    .pos = vec4_mul(&v->pos, c),
    .world_pos = vec4_mul(&v->world_pos, c),
    .normal = vec4_mul(&v->normal, c),
  };
}

PhongEffectGSOut phong_effect_gsout_mul_add(const PhongEffectGSOut* v, const PhongEffectGSOut* w, float c)
{
  return (PhongEffectGSOut){
    .pos = vec4_mul_add(&v->pos, &w->pos, c),
    .world_pos = vec4_mul_add(&v->world_pos, &w->world_pos, c),
    .normal = vec4_mul_add(&v->normal, &w->normal, c),
  };
}

PhongEffectGSOut phong_effect_gsout_interpolate(const PhongEffectGSOut* v, const PhongEffectGSOut* w, float alpha)
{
  return (PhongEffectGSOut){
    .pos = vec4_interpolate(&v->pos, &w->pos, alpha),
    .world_pos = vec4_interpolate(&v->world_pos, &w->world_pos, alpha),
    .normal = vec4_interpolate(&v->normal, &w->normal, alpha),
  };
}

PhongEffect phong_effect_make(const Graphics* graphics)
{
  return (PhongEffect){
    .graphics = graphics,
  };
}

void phong_effect_set_world(PhongEffect* effect, const Mat4* world)
{
  effect->world = *world;
  effect->proj_world = mat4_mul(&effect->projection, &effect->world);
}

void phong_effect_set_projection(PhongEffect* effect, const Mat4* projection)
{
  effect->projection = *projection;
  effect->proj_world = mat4_mul(&effect->projection, &effect->world);
}

void phong_effect_set_light_pos(PhongEffect* effect, const Vec3* light_pos)
{
  const Vec4 pos = vec4_make(light_pos->x, light_pos->y, light_pos->z, 1.0f);
  effect->light_pos = mat4_vec_mul(&effect->world, &pos);
}

void phong_effect_set_ambient_light(PhongEffect* effect, const Vec3* light)
{
  effect->ambient_light = *light;
}

void phong_effect_set_diffuse_light(PhongEffect* effect, const Vec3* light)
{
  effect->diffuse_light = *light;
}

void phong_effect_set_specular_light(PhongEffect* effect, const Vec3* light)
{
  effect->specular_light = *light;
}

void phong_effect_set_material_color(PhongEffect* effect, const Vec3* color)
{
  effect->material_color = *color;
}

void phong_effect_set_attenuation(PhongEffect* effect, float quadratic, float linear, float constant)
{
  effect->quadratic_attenuation = quadratic;
  effect->linear_attenuation = linear;
  effect->constant_attenuation = constant;
}

void phong_effect_set_light_coefficients(PhongEffect* effect, float ambient, float diffuse, float specular)
{
  effect->ambient_coeff = ambient;
  effect->diffuse_coeff = diffuse;
  effect->specular_coeff = specular;
}

void phong_effect_set_specular_power(PhongEffect* effect, float power)
{
  effect->specular_power = power;
}

void phong_effect_vertex_shader(const PhongEffect* effect, const PhongEffectVertex* in, PhongEffectVSOut* out)
{
  const Vec4 in_pos = vec4_make(in->pos.x, in->pos.y, in->pos.z, 1.0f);
  const Vec4 in_normal = vec4_make(in->normal.x, in->normal.y, in->normal.z, 0.0f);

  out->pos = mat4_vec_mul(&effect->proj_world, &in_pos);
  out->world_pos = mat4_vec_mul(&effect->world, &in_pos);
  out->normal = mat4_vec_mul(&effect->world, &in_normal);
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

  out0->world_pos = in0->world_pos;
  out1->world_pos = in1->world_pos;
  out2->world_pos = in2->world_pos;

  out0->normal = in0->normal;
  out1->normal = in1->normal;
  out2->normal = in2->normal;
}

PhongEffectGSOut phong_effect_screen_transform(const PhongEffect* effect, const PhongEffectGSOut* in)
{
  PhongEffectGSOut out;

  out.pos.x = in->pos.x / in->pos.w;
  out.pos.y = in->pos.y / in->pos.w;
  out.pos.z = in->pos.z / in->pos.w;
  out.pos.w = 1.0f;

  const size_t screen_width = effect->graphics->screen_width;
  const size_t screen_height = effect->graphics->screen_height;

  out.pos.x = (screen_width / 2.0f) * (out.pos.x + 1.0f);
  out.pos.y = (screen_height / 2.0f) * (-out.pos.y + 1.0f);

  out.normal = in->normal;
  out.world_pos = in->world_pos;

  return out;
}

Color phong_effect_pixel_shader(const PhongEffect* effect, const PhongEffectGSOut* in)
{
  const Vec4 normal = vec4_normalized(&in->normal);
  const Vec4 pos_to_light = vec4_sub(&effect->light_pos, &in->world_pos);
  const float dist = vec4_length(&pos_to_light);
  const Vec4 dir = vec4_normalized(&pos_to_light);

  const float attenuation = 1.0f / (effect->quadratic_attenuation * dist * dist + effect->linear_attenuation * dist +
                                    effect->constant_attenuation);

  const Vec3 diffuse =
    vec3_mul(&effect->diffuse_light, effect->diffuse_coeff * attenuation * fmax(0.0f, vec4_dot(&dir, &normal)));

  Vec4 r = vec4_mul(&normal, 2 * vec4_dot(&pos_to_light, &normal));
  r = vec4_sub(&r, &pos_to_light);
  r = vec4_normalized(&r);
  Vec4 v = vec4_mul(&in->world_pos, -1.0f);
  v = vec4_normalized(&v);

  const Vec3 specular =
    vec3_mul(&effect->specular_light,
             effect->specular_coeff * attenuation * pow(fmax(0.0f, vec4_dot(&r, &v)), effect->specular_power));

  Vec3 light = vec3_add(&effect->ambient_light, &diffuse);
  light = vec3_add(&light, &specular);

  Vec3 color = vec3_hadamard(&effect->material_color, &light);
  color = vec3_saturate(&color);
  color = vec3_mul(&color, 255.0f);

  return ((Color)color.x << 24) | ((Color)color.y << 16) | ((Color)color.z << 8) | 255;
}
