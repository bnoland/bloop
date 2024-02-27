#ifndef COLOR_CUBE_SCENE_H_
#define COLOR_CUBE_SCENE_H_

#include "graphics.h"
#include "depth_buffer.h"
#include "pipelines/color_pipeline.h"
#include "meshes/color_mesh.h"

typedef struct
{
  DepthBuffer* depth_buffer;
  ColorMesh mesh;
  ColorPipeline pipeline;
  float theta_x;
  float theta_y;
  float theta_z;
  float x;
  float y;
  float z;
} ColorCubeScene;

ColorCubeScene color_cube_scene_make(const Graphics* graphics);
void color_cube_scene_destroy(ColorCubeScene* scene);
void color_cube_scene_update(ColorCubeScene* scene, float dt);
void color_cube_scene_draw(ColorCubeScene* scene);

#endif
