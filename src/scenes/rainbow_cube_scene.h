#ifndef RAINBOW_CUBE_SCENE_H_
#define RAINBOW_CUBE_SCENE_H_

#include "graphics.h"
#include "pipelines/rainbow_pipeline.h"
#include "meshes/rainbow_mesh.h"

typedef struct
{
  RainbowMesh mesh;
  RainbowPipeline pipeline;
  float theta_x;
  float theta_y;
  float theta_z;
} RainbowCubeScene;

RainbowCubeScene rainbow_cube_scene_make(const Graphics* graphics);
void rainbow_cube_scene_destroy(RainbowCubeScene* scene);
void rainbow_cube_scene_update(RainbowCubeScene* scene, float dt);
void rainbow_cube_scene_draw(RainbowCubeScene* scene);

#endif
