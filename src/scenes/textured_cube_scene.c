#include "textured_cube_scene.h"

#include "matrix.h"
#include "utility.h"
#include "effects/texture_effect.h"

#include <SDL.h>
#include <stdlib.h>

static TextureMesh make_cube_mesh(float side);

TexturedCubeScene textured_cube_scene_make(const Graphics* graphics)
{
  DepthBuffer* depth_buffer = depth_buffer_make(graphics->screen_width, graphics->screen_height);

  Texture* texture = texture_make();
  texture_load_from_file(texture, "resources/rocky.png");

  TexturePipeline pipeline = texture_pipeline_make(graphics, depth_buffer);
  texture_effect_bind_texture(&pipeline.effect, texture);

  const Mat4 projection = mat4_projection(90.0f, 4.0f / 3.0f, 1.0f, 10.0f);
  texture_effect_bind_projection(&pipeline.effect, &projection);

  return (TexturedCubeScene){
    .depth_buffer = depth_buffer,
    .mesh = make_cube_mesh(1.0f),
    .texture = texture,
    .pipeline = pipeline,
    .theta_x = 0.0f,
    .theta_y = 0.0f,
    .theta_z = 0.0f,
    .x = 0.0f,
    .y = 0.0f,
    .z = -3.0f,
  };
}

void textured_cube_scene_destroy(TexturedCubeScene* scene)
{
  depth_buffer_destroy(scene->depth_buffer);
  texture_destroy(scene->texture);
  texture_mesh_destroy(&scene->mesh);
}

void textured_cube_scene_update(TexturedCubeScene* scene, float dt)
{
  const uint8_t* key_states = SDL_GetKeyboardState(NULL);

  const double angular_speed = 3.0;
  const double speed = 3.0;

  // Rotate around z-axis
  if (key_states[SDL_SCANCODE_E]) {
    scene->theta_z = wrap_angle(scene->theta_z + angular_speed * dt);
  }
  if (key_states[SDL_SCANCODE_D]) {
    scene->theta_z = wrap_angle(scene->theta_z - angular_speed * dt);
  }

  // Rotate around x-axis
  if (key_states[SDL_SCANCODE_Q]) {
    scene->theta_x = wrap_angle(scene->theta_x + angular_speed * dt);
  }
  if (key_states[SDL_SCANCODE_A]) {
    scene->theta_x = wrap_angle(scene->theta_x - angular_speed * dt);
  }

  // Rotate around y-axis
  if (key_states[SDL_SCANCODE_W]) {
    scene->theta_y = wrap_angle(scene->theta_y + angular_speed * dt);
  }
  if (key_states[SDL_SCANCODE_S]) {
    scene->theta_y = wrap_angle(scene->theta_y - angular_speed * dt);
  }

  // Move along z-axis
  if (key_states[SDL_SCANCODE_UP]) {
    scene->z -= speed * dt;
  }
  if (key_states[SDL_SCANCODE_DOWN]) {
    scene->z += speed * dt;
  }

  // Move along x-axis
  if (key_states[SDL_SCANCODE_LEFT]) {
    scene->x -= speed * dt;
  }
  if (key_states[SDL_SCANCODE_RIGHT]) {
    scene->x += speed * dt;
  }

  // Move along y-axis
  if (key_states[SDL_SCANCODE_PAGEDOWN]) {
    scene->y -= speed * dt;
  }
  if (key_states[SDL_SCANCODE_PAGEUP]) {
    scene->y += speed * dt;
  }
}

void textured_cube_scene_draw(TexturedCubeScene* scene)
{
  const Mat4 rotation_x = mat4_rotation_x(scene->theta_x);
  const Mat4 rotation_y = mat4_rotation_y(scene->theta_y);
  const Mat4 rotation_z = mat4_rotation_z(scene->theta_z);
  const Mat4 translation = mat4_translation(scene->x, scene->y, scene->z);

  Mat4 world_view = translation;
  world_view = mat4_mul(&world_view, &rotation_x);
  world_view = mat4_mul(&world_view, &rotation_y);
  world_view = mat4_mul(&world_view, &rotation_z);

  texture_effect_bind_world_view(&scene->pipeline.effect, &world_view);

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
