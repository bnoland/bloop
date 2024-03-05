#ifndef TEAPOT_SCENE_H_
#define TEAPOT_SCENE_H_

#include "graphics.h"
#include "depth_buffer.h"
#include "vector.h"
#include "pipelines/phong_pipeline.h"
#include "meshes/normal_mesh.h"

typedef struct
{
  DepthBuffer* depth_buffer;
  NormalMesh mesh;
  PhongPipeline pipeline;
  Vec4 light_pos_base;
  Vec4 light_pos;
  Vec3 camera_pos;
  Vec3 camera_angles;
  Vec3 camera_forward_base;
  Vec3 camera_left_base;
  Vec3 camera_forward;
  Vec3 camera_left;
} TeapotScene;

TeapotScene teapot_scene_make(const Graphics* graphics);
void teapot_scene_destroy(TeapotScene* scene);
void teapot_scene_update(TeapotScene* scene, float dt);
void teapot_scene_draw(TeapotScene* scene);

#endif
