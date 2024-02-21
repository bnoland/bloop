#include "graphics.h"
#include "mesh.h"
#include "pipeline.h"
#include "dynlist.h"

#include <SDL.h>
#include <stdlib.h>
#include <stdbool.h>

int main(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  const int screen_width = 800;
  const int screen_height = 600;

  SDL_Window* window = SDL_CreateWindow(
    "Bloop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_SetWindowMinimumSize(window, screen_width, screen_height);
  SDL_RenderSetLogicalSize(renderer, screen_width, screen_height);
  SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

  SDL_Texture* screen_texture =
    SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, screen_width, screen_height);
  if (screen_texture == NULL) {
    fprintf(stderr, "Failed to create screen texture: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  Mesh mesh;
  if (!mesh_load_from_file(&mesh, "resources/suzanne.obj", true, false, true)) {
    fprintf(stderr, "Failed to load mesh.\n");
    return EXIT_FAILURE;
  }

  DynList vertices;
  dyn_list_init(&vertices, sizeof(Vertex));
  for (size_t i = 0; i < mesh.positions.size; i++) {
    Vertex vertex = { .pos = *(Vec3*)dyn_list_at(&mesh.positions, i) };
    dyn_list_add(&vertices, &vertex);
  }

  DynList indices;
  dyn_list_init(&indices, sizeof(size_t));
  for (size_t i = 0; i < mesh.vertices.size; i++) {
    const MeshVertex* mesh_vert = (MeshVertex*)dyn_list_at(&mesh.vertices, i);
    dyn_list_add(&indices, &mesh_vert->pos_index);
  }

  Graphics graphics;
  graphics_init(&graphics, screen_width, screen_height);

  Pipeline pipeline;
  pipeline_init(&pipeline, &graphics, NULL, NULL, NULL);

  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        return EXIT_SUCCESS;
      }

      if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        return EXIT_SUCCESS;
      }
    }

    graphics_clear(&graphics, 0x000000ff);
    // pipeline_draw(&pipeline, &vertices, &indices);

    const Vec3 t0[] = { { .x = 10, .y = 70 }, { .x = 50, .y = 160 }, { .x = 70, .y = 80 } };
    const Vec3 t1[] = { { .x = 180, .y = 50 }, { .x = 150, .y = 1 }, { .x = 70, .y = 180 } };
    const Vec3 t2[] = { { .x = 180, .y = 150 }, { .x = 120, .y = 160 }, { .x = 130, .y = 180 } };
    const Vec3 t3[] = { { .x = 200, .y = 200 }, { .x = 200, .y = 300 }, { .x = 300, .y = 200 } };
    graphics_draw_triangle(&graphics, &t0[0], &t0[1], &t0[2], 0xff0000ff);
    graphics_draw_triangle(&graphics, &t1[0], &t1[1], &t1[2], 0xffffffff);
    graphics_draw_triangle(&graphics, &t2[0], &t2[1], &t2[2], 0x00ff00ff);
    graphics_draw_triangle(&graphics, &t3[0], &t3[1], &t3[2], 0x0000ffff);

    SDL_RenderClear(renderer);
    SDL_UpdateTexture(screen_texture, NULL, graphics.pixel_buffer, screen_width * sizeof(Color));
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  dyn_list_destroy(&vertices);
  dyn_list_destroy(&indices);

  graphics_destroy(&graphics);
  mesh_destroy(&mesh);

  SDL_DestroyTexture(screen_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
