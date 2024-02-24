#include "graphics.h"
#include "simple_mesh.h"
#include "simple_pipeline.h"
#include "matrix.h"
#include "utility.h"

#include <SDL.h>
#include <stdlib.h>
#include <stdbool.h>

static void make_cube_mesh(SimpleMesh* mesh, float side);
static void vertex_shader(const Vertex* in, Vertex* out);
void geom_shader(const Vertex* in0,
                 const Vertex* in1,
                 const Vertex* in2,
                 Vertex* out0,
                 Vertex* out1,
                 Vertex* out2,
                 size_t triangle_index);

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

  SimpleMesh mesh;
  make_cube_mesh(&mesh, 1.0f);

  Graphics graphics;
  graphics_init(&graphics, screen_width, screen_height);

  SimplePipeline pipeline;
  simple_pipeline_init(&pipeline, &graphics, vertex_shader, NULL, NULL);

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
    simple_pipeline_draw(&pipeline, &mesh);

    SDL_RenderClear(renderer);
    SDL_UpdateTexture(screen_texture, NULL, graphics.pixel_buffer, screen_width * sizeof(Color));
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  graphics_destroy(&graphics);
  simple_mesh_destroy(&mesh);

  SDL_DestroyTexture(screen_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}

static void make_cube_mesh(SimpleMesh* mesh, float side)
{
  const float half_side = side / 2.0f;

  // XXX: Get rid of the stupid warnings about missing braces.
  const Vertex vertices[] = {
    // Near side
    { .pos = { -half_side, -half_side, -half_side } },
    { .pos = { half_side, -half_side, -half_side } },
    { .pos = { -half_side, half_side, -half_side } },
    { .pos = { half_side, half_side, -half_side } },

    // Far side
    { .pos = { -half_side, -half_side, half_side } },
    { .pos = { half_side, -half_side, half_side } },
    { .pos = { -half_side, half_side, half_side } },
    { .pos = { half_side, half_side, half_side } },

    // Left side
    { .pos = { -half_side, -half_side, -half_side } },
    { .pos = { -half_side, half_side, -half_side } },
    { .pos = { -half_side, -half_side, half_side } },
    { .pos = { -half_side, half_side, half_side } },

    // Right side
    { .pos = { half_side, -half_side, -half_side } },
    { .pos = { half_side, half_side, -half_side } },
    { .pos = { half_side, -half_side, half_side } },
    { .pos = { half_side, half_side, half_side } },

    // Bottom side
    { .pos = { -half_side, -half_side, -half_side } },
    { .pos = { half_side, -half_side, -half_side } },
    { .pos = { -half_side, -half_side, half_side } },
    { .pos = { half_side, -half_side, half_side } },

    // Top side
    { .pos = { -half_side, half_side, -half_side } },
    { .pos = { half_side, half_side, -half_side } },
    { .pos = { -half_side, half_side, half_side } },
    { .pos = { half_side, half_side, half_side } },
  };

  const size_t indices[] = {
    0,  2,  1,  2,  3,  1,  4,  5,  7,  4,  7,  6,  8,  10, 9,  10, 11, 9,
    12, 13, 15, 12, 15, 14, 16, 17, 18, 18, 17, 19, 20, 23, 21, 20, 22, 23,
  };

  const size_t num_vertices = 24;
  const size_t num_indices = 36;

  simple_mesh_load_from_arrays(mesh, true, vertices, num_vertices, indices, num_indices);
}

static void vertex_shader(const Vertex* in, Vertex* out)
{
  // Mat4 rotation;
  // mat4_rotation_y(&rotation, M_PI / 4.0f);

  // Mat4 translation;
  // mat4_translation(&translation, 0.0f, 0.0f, -2.0f);

  // Mat4 transform;
  // mat4_mul(&transform, &translation, &rotation);

  // mat4_vec_mul(&out->pos, &transform, &in->pos);

  // out->pos.x /= -out->pos.z;
  // out->pos.y /= -out->pos.z;
}
