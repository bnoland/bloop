#ifndef TEXTURED_CUBE_SCENE_H_
#define TEXTURED_CUBE_SCENE_H_

#include "graphics.h"
#include "depth_buffer.h"
#include "texture.h"
#include "pipelines/texture_pipeline.h"
#include "meshes/texture_mesh.h"

typedef struct
{
  DepthBuffer* depth_buffer;
  TextureMesh mesh;
  TexturePipeline pipeline;
  Texture* texture;
  float theta_x;
  float theta_y;
  float theta_z;
  float x;
  float y;
  float z;
} TexturedCubeScene;

TexturedCubeScene textured_cube_scene_make(const Graphics* graphics);
void textured_cube_scene_destroy(TexturedCubeScene* scene);
void textured_cube_scene_update(TexturedCubeScene* scene, float dt);
void textured_cube_scene_draw(TexturedCubeScene* scene);

#endif
