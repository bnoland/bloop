#include "gouraud_scene.h"

#include "matrix.h"
#include "utility.h"
#include "effects/gouraud_effect.h"

#include <SDL.h>
#include <stdlib.h>

GouraudScene gouraud_scene_make(const Graphics* graphics)
{
  DepthBuffer* depth_buffer = depth_buffer_make(graphics->screen_width, graphics->screen_height);

  GouraudMesh mesh = gouraud_mesh_make();
  gouraud_mesh_load_from_file(&mesh, "resources/suzanne.obj", false, true);

  return (GouraudScene){
    .depth_buffer = depth_buffer,
    .mesh = mesh,
    .pipeline = gouraud_pipeline_make(graphics, depth_buffer),
    .theta_x = 0.0f,
    .theta_y = 0.0f,
    .theta_z = 0.0f,
    .x = 0.0f,
    .y = 0.0f,
    .z = -3.0f,
  };
}

void gouraud_scene_destroy(GouraudScene* scene)
{
  depth_buffer_destroy(scene->depth_buffer);
  gouraud_mesh_destroy(&scene->mesh);
}

void gouraud_scene_update(GouraudScene* scene, float dt)
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

void gouraud_scene_draw(GouraudScene* scene)
{
  // XXX: Don't recompute on each draw.
  const Mat4 rotation_x = mat4_rotation_x(scene->theta_x);
  const Mat4 rotation_y = mat4_rotation_y(scene->theta_y);
  const Mat4 rotation_z = mat4_rotation_z(scene->theta_z);
  const Mat4 translation = mat4_translation(scene->x, scene->y, scene->z);
  const Mat4 projection = mat4_projection(90.0f, 4.0f / 3.0f, 1.0f, 10.0f);

  Mat4 world_view = translation;
  world_view = mat4_mul(&world_view, &rotation_x);
  world_view = mat4_mul(&world_view, &rotation_y);
  world_view = mat4_mul(&world_view, &rotation_z);

  gouraud_effect_bind_world_view(&scene->pipeline.effect, &world_view);
  gouraud_effect_bind_projection(&scene->pipeline.effect, &projection);

  gouraud_pipeline_draw(&scene->pipeline, &scene->mesh);
}
