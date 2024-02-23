#include "graphics.h"
#include "mesh.h"
#include "pipeline.h"

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

    SDL_RenderClear(renderer);
    SDL_UpdateTexture(screen_texture, NULL, graphics.pixel_buffer, screen_width * sizeof(Color));
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  graphics_destroy(&graphics);
  mesh_destroy(&mesh);

  SDL_DestroyTexture(screen_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
