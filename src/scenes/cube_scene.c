#include "cube_scene.h"

#include "matrix.h"
#include "vector.h"
#include "utility.h"

static SimpleMesh make_cube_mesh(float side);

CubeScene cube_scene_make(Graphics* graphics)
{
  return (CubeScene){
    .mesh = make_cube_mesh(1.0f),
    .pipeline = simple_pipeline_make(graphics),
    .theta_x = 0.0f,
    .theta_y = M_PI / 4.0f,
    .theta_z = 0.0f,
  };
}

void cube_scene_destroy(CubeScene* scene)
{
  simple_mesh_destroy(&scene->mesh);
}

void cube_scene_update(CubeScene* scene, float dt)
{
  // XXX: Handle key presses to rotate cube.
  // XXX: Include x-axis + z-axis rotation.
  const Mat4 rotation = mat4_rotation_y(scene->theta_y);
  const Mat4 translation = mat4_translation(0.0f, 0.0f, -2.0f);
  const Mat4 transform = mat4_mul(&translation, &rotation);
  simple_effect_bind_transform(&scene->pipeline.effect, &transform);
}

void cube_scene_draw(const CubeScene* scene)
{
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
