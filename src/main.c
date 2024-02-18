#include "graphics.h"
#include "model.h"

#include <SDL.h>
#include <stdlib.h>
#include <stdbool.h>

int main(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Window* window = SDL_CreateWindow(
    "Bloop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_screen_width, g_screen_height, SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_SetWindowMinimumSize(window, g_screen_width, g_screen_height);
  SDL_RenderSetLogicalSize(renderer, g_screen_width, g_screen_height);
  SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

  SDL_Texture* screen_texture =
    SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, g_screen_width, g_screen_height);
  if (screen_texture == NULL) {
    fprintf(stderr, "Failed to create screen texture: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  Model model;
  model_load(&model, "resources/teapot.obj", true);
  model_free(&model);

  initialize_graphics();

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

    clear(0xffffffff);

    SDL_RenderClear(renderer);
    SDL_UpdateTexture(screen_texture, NULL, g_pixel_buffer, g_screen_width * sizeof(Color));
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  deinitialize_graphics();

  SDL_DestroyTexture(screen_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
