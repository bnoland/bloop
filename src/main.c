#include "graphics.h"
#include "vector.h"

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

    clear(0x000000ff);

    const Vec3 t0[] = { { .x = 10, .y = 70 }, { .x = 50, .y = 160 }, { .x = 70, .y = 80 } };
    const Vec3 t1[] = { { .x = 180, .y = 50 }, { .x = 150, .y = 1 }, { .x = 70, .y = 180 } };
    const Vec3 t2[] = { { .x = 180, .y = 150 }, { .x = 120, .y = 160 }, { .x = 130, .y = 180 } };
    const Vec3 t3[] = { { .x = 200, .y = 200 }, { .x = 200, .y = 300 }, { .x = 300, .y = 200 } };
    draw_triangle(&t0[0], &t0[1], &t0[2], 0xff0000ff);
    draw_triangle(&t1[0], &t1[1], &t1[2], 0xffffffff);
    draw_triangle(&t2[0], &t2[1], &t2[2], 0x00ff00ff);
    draw_triangle(&t3[0], &t3[1], &t3[2], 0x0000ffff);

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
