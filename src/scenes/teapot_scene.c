#include "teapot_scene.h"

#include "matrix.h"
#include "utility.h"
#include "effects/phong_effect.h"

#include <SDL.h>
#include <stdlib.h>

static void teapot_scene_update_camera(TeapotScene* scene);

TeapotScene teapot_scene_make(const Graphics* graphics)
{
  DepthBuffer* depth_buffer = depth_buffer_make(graphics->screen_width, graphics->screen_height);
  PhongPipeline pipeline = phong_pipeline_make(graphics, depth_buffer);

  // XXX: Will probably want to compute normals from faces + interpolate.
  NormalMesh mesh = normal_mesh_make();
  normal_mesh_load_from_file(&mesh, "resources/teapot.obj", false, false);

  const Mat4 projection = mat4_projection(90.0f, 4.0f / 3.0f, 0.01f, 10.0f);
  phong_effect_bind_projection(&pipeline.effect, &projection);

  const Vec3 camera_forward_base = vec3_make(0.0f, 0.0f, -1.0f);
  const Vec3 camera_left_base = vec3_make(-1.0f, 0.0f, 0.0f);

  TeapotScene scene = {
    .depth_buffer = depth_buffer,
    .mesh = mesh,
    .pipeline = pipeline,
    .camera_pos = vec3_make(0.0f, 0.0f, 8.0f),
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
  normal_mesh_destroy(&scene->mesh);
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

  phong_effect_bind_world_view(&scene->pipeline.effect, &world_view);
}

void teapot_scene_draw(TeapotScene* scene)
{
  phong_pipeline_draw(&scene->pipeline, &scene->mesh);
}
