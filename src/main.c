#include "graphics.h"
#include "vector.h"
#include "mesh.h"

#include <SDL.h>
#include <stdlib.h>
#include <stdbool.h>

static void transform_to_screen(Vec3* v);

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

  Mesh mesh;
  if (!mesh_load_from_file(&mesh, "resources/suzanne.obj", true, false, true)) {
    fprintf(stderr, "Failed to load mesh.\n");
    return EXIT_FAILURE;
  }

  const size_t num_mesh_vertices = mesh.vertices.size;

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

    for (size_t i = 0; i < num_mesh_vertices / 3; i++) {
      const Vertex* v0 = (Vertex*)dyn_list_at(&mesh.vertices, 3 * i);
      const Vertex* v1 = (Vertex*)dyn_list_at(&mesh.vertices, 3 * i + 1);
      const Vertex* v2 = (Vertex*)dyn_list_at(&mesh.vertices, 3 * i + 2);

      Vec3 p0 = *(Vec3*)dyn_list_at(&mesh.positions, v0->pos_index);
      Vec3 p1 = *(Vec3*)dyn_list_at(&mesh.positions, v1->pos_index);
      Vec3 p2 = *(Vec3*)dyn_list_at(&mesh.positions, v2->pos_index);

      transform_to_screen(&p0);
      transform_to_screen(&p1);
      transform_to_screen(&p2);

      draw_triangle(&p0, &p1, &p2, 0xffffffff);
    }

    SDL_RenderClear(renderer);
    SDL_UpdateTexture(screen_texture, NULL, g_pixel_buffer, g_screen_width * sizeof(Color));
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  deinitialize_graphics();

  mesh_free(&mesh);

  SDL_DestroyTexture(screen_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}

// XXX: Should go elsewhere.
static void transform_to_screen(Vec3* v)
{
  v->x = (g_screen_width / 2.0f) * v->x + (g_screen_width / 2.0f);
  v->y = (-g_screen_height / 2.0f) * v->y + (g_screen_height / 2.0f);
}
