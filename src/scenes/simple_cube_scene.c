#include "simple_cube_scene.h"

#include "matrix.h"
#include "vector.h"
#include "utility.h"

#include <SDL.h>

static SimpleMesh make_cube_mesh(float side);

SimpleCubeScene simple_cube_scene_make(const Graphics* graphics)
{
  return (SimpleCubeScene){
    .mesh = make_cube_mesh(1.0f),
    .pipeline = simple_pipeline_make(graphics),
    .theta_x = 0.0f,
    .theta_y = 0.0f,
    .theta_z = 0.0f,
  };
}

void simple_cube_scene_destroy(SimpleCubeScene* scene)
{
  simple_mesh_destroy(&scene->mesh);
}

void simple_cube_scene_update(SimpleCubeScene* scene, float dt)
{
  const uint8_t* key_states = SDL_GetKeyboardState(NULL);

  const double angular_speed = 6.0;

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
}

void simple_cube_scene_draw(SimpleCubeScene* scene)
{
  const Mat4 rotation_x = mat4_rotation_x(scene->theta_x);
  const Mat4 rotation_y = mat4_rotation_y(scene->theta_y);
  const Mat4 rotation_z = mat4_rotation_z(scene->theta_z);
  const Mat4 translation = mat4_translation(0.0f, 0.0f, -2.0f);

  Mat4 transform = mat4_mul(&translation, &rotation_x);
  transform = mat4_mul(&transform, &rotation_y);
  transform = mat4_mul(&transform, &rotation_z);
  simple_effect_bind_transform(&scene->pipeline.effect, &transform);

  simple_pipeline_draw(&scene->pipeline, &scene->mesh);
}

static SimpleMesh make_cube_mesh(float side)
{
  const float half_side = side / 2.0f;

  const SimpleEffectVertex vertices[] = {
    // Near side
    { .pos = vec3_make(-half_side, -half_side, -half_side) },
    { .pos = vec3_make(half_side, -half_side, -half_side) },
    { .pos = vec3_make(-half_side, half_side, -half_side) },
    { .pos = vec3_make(half_side, half_side, -half_side) },

    // Far side
    { .pos = vec3_make(-half_side, -half_side, half_side) },
    { .pos = vec3_make(half_side, -half_side, half_side) },
    { .pos = vec3_make(-half_side, half_side, half_side) },
    { .pos = vec3_make(half_side, half_side, half_side) },

    // Left side
    { .pos = vec3_make(-half_side, -half_side, -half_side) },
    { .pos = vec3_make(-half_side, half_side, -half_side) },
    { .pos = vec3_make(-half_side, -half_side, half_side) },
    { .pos = vec3_make(-half_side, half_side, half_side) },

    // Right side
    { .pos = vec3_make(half_side, -half_side, -half_side) },
    { .pos = vec3_make(half_side, half_side, -half_side) },
    { .pos = vec3_make(half_side, -half_side, half_side) },
    { .pos = vec3_make(half_side, half_side, half_side) },

    // Bottom side
    { .pos = vec3_make(-half_side, -half_side, -half_side) },
    { .pos = vec3_make(half_side, -half_side, -half_side) },
    { .pos = vec3_make(-half_side, -half_side, half_side) },
    { .pos = vec3_make(half_side, -half_side, half_side) },

    // Top side
    { .pos = vec3_make(-half_side, half_side, -half_side) },
    { .pos = vec3_make(half_side, half_side, -half_side) },
    { .pos = vec3_make(-half_side, half_side, half_side) },
    { .pos = vec3_make(half_side, half_side, half_side) },
  };

  const size_t indices[] = {
    0,  2,  1,  2,  3,  1,  4,  5,  7,  4,  7,  6,  8,  10, 9,  10, 11, 9,
    12, 13, 15, 12, 15, 14, 16, 17, 18, 18, 17, 19, 20, 23, 21, 20, 22, 23,
  };

  const size_t num_vertices = 24;
  const size_t num_indices = 36;

  SimpleMesh mesh = simple_mesh_make();
  simple_mesh_load_from_arrays(&mesh, vertices, num_vertices, indices, num_indices);
  return mesh;
}