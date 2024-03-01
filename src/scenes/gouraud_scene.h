#ifndef GOURAUD_SCENE_H_
#define GOURAUD_SCENE_H_

#include "graphics.h"
#include "depth_buffer.h"
#include "pipelines/gouraud_pipeline.h"
#include "meshes/gouraud_mesh.h"

typedef struct
{
  DepthBuffer* depth_buffer;
  GouraudMesh mesh;
  GouraudPipeline pipeline;
  float theta_x;
  float theta_y;
  float theta_z;
  float x;
  float y;
  float z;
} GouraudScene;

GouraudScene gouraud_scene_make(const Graphics* graphics);
void gouraud_scene_destroy(GouraudScene* scene);
void gouraud_scene_update(GouraudScene* scene, float dt);
void gouraud_scene_draw(GouraudScene* scene);

#endif
