#include "teapot_scene.h"

#include "matrix.h"
#include "utility.h"
#include "effects/texture_effect.h"

#include <SDL.h>
#include <stdlib.h>

static TextureMesh make_cube_mesh(float side);
static void teapot_scene_update_camera(TeapotScene* scene);

TeapotScene teapot_scene_make(const Graphics* graphics)
{
  DepthBuffer* depth_buffer = depth_buffer_make(graphics->screen_width, graphics->screen_height);

  Texture* texture = texture_make();
  texture_load_from_file(texture, "resources/rocky.png");

  TexturePipeline pipeline = texture_pipeline_make(graphics, depth_buffer);
  texture_effect_bind_texture(&pipeline.effect, texture);

  const Mat4 projection = mat4_projection(90.0f, 4.0f / 3.0f, 0.01f, 10.0f);
  texture_effect_bind_projection(&pipeline.effect, &projection);

  const Vec3 camera_forward_base = vec3_make(0.0f, 0.0f, -1.0f);
  const Vec3 camera_left_base = vec3_make(-1.0f, 0.0f, 0.0f);

  TeapotScene scene = {
    .depth_buffer = depth_buffer,
    .mesh = make_cube_mesh(1.0f),
    .texture = texture,
    .pipeline = pipeline,
    .camera_pos = vec3_make(0.0f, 0.0f, 2.0f),
    .camera_angles = vec3_make(0.0f, 0.0f, 0.0f),
    .camera_forward_base = camera_forward_base,
    .camera_left_base = camera_left_base,
    .camera_forward = camera_forward_base,
    .camera_left = camera_left_base,
  };

  teapot_scene_update_camera(&scene);

  return scene;
}

void teapot_scene_destroy(TeapotScene* scene)
{
  depth_buffer_destroy(scene->depth_buffer);
  texture_destroy(scene->texture);
  texture_mesh_destroy(&scene->mesh);
}

void teapot_scene_update(TeapotScene* scene, float dt)
{
  const uint8_t* key_states = SDL_GetKeyboardState(NULL);

  const double angular_speed = 3.0f;
  const double speed = 3.0f;

  bool did_update = false;

  if (key_states[SDL_SCANCODE_W]) {
    scene->camera_pos = vec3_mul_add(&scene->camera_pos, &scene->camera_forward, speed * dt);
    did_update = true;
  }
  if (key_states[SDL_SCANCODE_S]) {
    scene->camera_pos = vec3_mul_add(&scene->camera_pos, &scene->camera_forward, -speed * dt);
    did_update = true;
  }
  if (key_states[SDL_SCANCODE_A]) {
    scene->camera_pos = vec3_mul_add(&scene->camera_pos, &scene->camera_left, speed * dt);
    did_update = true;
  }
  if (key_states[SDL_SCANCODE_D]) {
    scene->camera_pos = vec3_mul_add(&scene->camera_pos, &scene->camera_left, -speed * dt);
    did_update = true;
  }

  if (key_states[SDL_SCANCODE_LEFT]) {
    scene->camera_angles.y = wrap_angle(scene->camera_angles.y + angular_speed * dt);
    did_update = true;
  }
  if (key_states[SDL_SCANCODE_RIGHT]) {
    scene->camera_angles.y = wrap_angle(scene->camera_angles.y - angular_speed * dt);
    did_update = true;
  }

  if (did_update) {
    teapot_scene_update_camera(scene);
  }
}

static void teapot_scene_update_camera(TeapotScene* scene)
{
  const Mat3 camera_rot_y = mat3_rotation_y(scene->camera_angles.y);
  scene->camera_forward = mat3_vec_mul(&camera_rot_y, &scene->camera_forward_base);
  scene->camera_left = mat3_vec_mul(&camera_rot_y, &scene->camera_left_base);

  const Mat4 world_rot_x = mat4_rotation_x(-scene->camera_angles.x);
  const Mat4 world_rot_y = mat4_rotation_y(-scene->camera_angles.y);
  const Mat4 world_rot_z = mat4_rotation_z(-scene->camera_angles.z);
  const Mat4 world_trans = mat4_translation(-scene->camera_pos.x, -scene->camera_pos.y, -scene->camera_pos.z);

  Mat4 world_view = world_rot_x;
  world_view = mat4_mul(&world_view, &world_rot_y);
  world_view = mat4_mul(&world_view, &world_rot_z);
  world_view = mat4_mul(&world_view, &world_trans);

  texture_effect_bind_world_view(&scene->pipeline.effect, &world_view);
}

void teapot_scene_draw(TeapotScene* scene)
{
  texture_pipeline_draw(&scene->pipeline, &scene->mesh);
}

static TextureMesh make_cube_mesh(float side)
{
  const float half_side = side / 2.0f;

  const TextureEffectVertex vertices[] = {
    // Far side
    { .pos = vec3_make(-half_side, -half_side, -half_side), .uv = vec2_make(0.0f, 0.0f) },
    { .pos = vec3_make(half_side, -half_side, -half_side), .uv = vec2_make(1.0f, 0.0f) },
    { .pos = vec3_make(-half_side, half_side, -half_side), .uv = vec2_make(0.0f, 1.0f) },
    { .pos = vec3_make(half_side, half_side, -half_side), .uv = vec2_make(1.0f, 1.0f) },

    // Near side
    { .pos = vec3_make(-half_side, -half_side, half_side), .uv = vec2_make(0.0f, 0.0f) },
    { .pos = vec3_make(half_side, -half_side, half_side), .uv = vec2_make(1.0f, 0.0f) },
    { .pos = vec3_make(-half_side, half_side, half_side), .uv = vec2_make(0.0f, 1.0f) },
    { .pos = vec3_make(half_side, half_side, half_side), .uv = vec2_make(1.0f, 1.0f) },

    // Left side
    { .pos = vec3_make(-half_side, -half_side, -half_side), .uv = vec2_make(0.0f, 0.0f) },
    { .pos = vec3_make(-half_side, half_side, -half_side), .uv = vec2_make(1.0f, 0.0f) },
    { .pos = vec3_make(-half_side, -half_side, half_side), .uv = vec2_make(0.0f, 1.0f) },
    { .pos = vec3_make(-half_side, half_side, half_side), .uv = vec2_make(1.0f, 1.0f) },

    // Right side
    { .pos = vec3_make(half_side, -half_side, -half_side), .uv = vec2_make(0.0f, 0.0f) },
    { .pos = vec3_make(half_side, half_side, -half_side), .uv = vec2_make(1.0f, 0.0f) },
    { .pos = vec3_make(half_side, -half_side, half_side), .uv = vec2_make(0.0f, 1.0f) },
    { .pos = vec3_make(half_side, half_side, half_side), .uv = vec2_make(1.0f, 1.0f) },

    // Bottom side
    { .pos = vec3_make(-half_side, -half_side, -half_side), .uv = vec2_make(0.0f, 0.0f) },
    { .pos = vec3_make(half_side, -half_side, -half_side), .uv = vec2_make(1.0f, 0.0f) },
    { .pos = vec3_make(-half_side, -half_side, half_side), .uv = vec2_make(0.0f, 1.0f) },
    { .pos = vec3_make(half_side, -half_side, half_side), .uv = vec2_make(1.0f, 1.0f) },

    // Top side
    { .pos = vec3_make(-half_side, half_side, -half_side), .uv = vec2_make(0.0f, 0.0f) },
    { .pos = vec3_make(half_side, half_side, -half_side), .uv = vec2_make(1.0f, 0.0f) },
    { .pos = vec3_make(-half_side, half_side, half_side), .uv = vec2_make(0.0f, 1.0f) },
    { .pos = vec3_make(half_side, half_side, half_side), .uv = vec2_make(1.0f, 1.0f) },
  };

  const size_t indices[] = {
    0,  2,  1,  2,  3,  1,  4,  5,  7,  4,  7,  6,  8,  10, 9,  10, 11, 9,
    12, 13, 15, 12, 15, 14, 16, 17, 18, 18, 17, 19, 20, 23, 21, 20, 22, 23,
  };

  const size_t num_vertices = 24;
  const size_t num_indices = 36;

  TextureMesh mesh = texture_mesh_make();
  texture_mesh_load_from_arrays(&mesh, vertices, num_vertices, indices, num_indices);
  return mesh;
}
