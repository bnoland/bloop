#ifndef CUBE_SCENE_H_
#define CUBE_SCENE_H_

#include "graphics.h"
#include "pipelines/simple_pipeline.h"

typedef struct
{
  SimpleMesh mesh;
  SimplePipeline pipeline;
  float theta_x;
  float theta_y;
  float theta_z;
} CubeScene;

CubeScene cube_scene_make(Graphics* graphics);
void cube_scene_destroy(CubeScene* scene);
void cube_scene_update(CubeScene* scene, float dt);
void cube_scene_draw(const CubeScene* scene);

#endif
