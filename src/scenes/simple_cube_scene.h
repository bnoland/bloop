#ifndef SIMPLE_CUBE_SCENE_H_
#define SIMPLE_CUBE_SCENE_H_

#include "graphics.h"
#include "pipelines/simple_pipeline.h"

typedef struct
{
  SimpleMesh mesh;
  SimplePipeline pipeline;
  float theta_x;
  float theta_y;
  float theta_z;
} SimpleCubeScene;

SimpleCubeScene simple_cube_scene_make(const Graphics* graphics);
void simple_cube_scene_destroy(SimpleCubeScene* scene);
void simple_cube_scene_update(SimpleCubeScene* scene, float dt);
void simple_cube_scene_draw(SimpleCubeScene* scene);

#endif
