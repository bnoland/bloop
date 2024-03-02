#include "graphics.h"
#include "scenes/textured_cube_scene.h"

#include <SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static void update_fps_counter(SDL_Window* window);

int main(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  const int screen_width = 800;
  const int screen_height = 600;

  SDL_Window* window = SDL_CreateWindow(
    "", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_RESIZABLE);
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

  Graphics graphics = graphics_make(screen_width, screen_height);
  TexturedCubeScene scene = textured_cube_scene_make(&graphics);

  float last_time = 0.0f;

  while (true) {
    const float current_time = SDL_GetTicks() / 1000.0f;
    const float dt = current_time - last_time;
    last_time = current_time;

    update_fps_counter(window);

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
    textured_cube_scene_update(&scene, dt);
    textured_cube_scene_draw(&scene);

    SDL_RenderClear(renderer);
    SDL_UpdateTexture(screen_texture, NULL, graphics.pixel_buffer, screen_width * sizeof(Color));
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  textured_cube_scene_destroy(&scene);
  graphics_destroy(&graphics);

  SDL_DestroyTexture(screen_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}

static void update_fps_counter(SDL_Window* window)
{
  static uint32_t last_time = 0;
  static unsigned int frame_count = 0;

  const uint32_t current_time = SDL_GetTicks();
  const uint32_t delta_time = current_time - last_time;
  frame_count++;

  if (delta_time >= 500) {
    char title[128];
    snprintf(title, sizeof(title), "%.2f FPS", (float)frame_count / delta_time * 1000.0f);
    SDL_SetWindowTitle(window, title);
    last_time = current_time;
    frame_count = 0;
  }
}
