#ifndef SOLID_CUBE_SCENE_H_
#define SOLID_CUBE_SCENE_H_

#include "graphics.h"
#include "pipelines/default_pipeline.h"
#include "meshes/default_mesh.h"

typedef struct
{
  DefaultMesh mesh;
  DefaultPipeline pipeline;
  float theta_x;
  float theta_y;
  float theta_z;
} SolidCubeScene;

SolidCubeScene solid_cube_scene_make(const Graphics* graphics);
void solid_cube_scene_destroy(SolidCubeScene* scene);
void solid_cube_scene_update(SolidCubeScene* scene, float dt);
void solid_cube_scene_draw(SolidCubeScene* scene);

#endif
