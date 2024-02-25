#include "graphics.h"
#include "simple_mesh.h"
#include "simple_pipeline.h"
#include "matrix.h"
#include "utility.h"

#include <SDL.h>
#include <stdlib.h>
#include <stdbool.h>

static SimpleMesh make_cube_mesh(float side);
static void vertex_shader(const Vertex* in, VSOut* out);
void geom_shader(const VSOut* in0,
                 const VSOut* in1,
                 const VSOut* in2,
                 GSOut* out0,
                 GSOut* out1,
                 GSOut* out2,
                 size_t triangle_index);
Color pixel_shader(const GSOut* in);

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

  SimpleMesh mesh = make_cube_mesh(1.0f);

  Graphics graphics = graphics_make(screen_width, screen_height);

  SimplePipeline pipeline = simple_pipeline_make(&graphics, vertex_shader, geom_shader, pixel_shader);

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

static SimpleMesh make_cube_mesh(float side)
{
  const float half_side = side / 2.0f;

  const Vertex vertices[] = {
    // Near side
    { .pos = vec3_make(-half_side, -half_side, -half_side) },
    { .pos = vec3_make(half_side, -half_side, -half_side) },
    { .pos = vec3_make(-half_side, half_side, -half_side) },
    { .pos = vec3_make(half_side, half_side, -half_side) },

    // Far side
    { .pos = vec3_make(-half_side, -half_side, half_side) },
    { .pos = vec3_make(half_side, -half_side, half_side) },
    { .pos = vec3_make(-half_side, half_side, half_side) },
    { .pos = vec3_make(half_side, half_side, half_side) },

    // Left side
    { .pos = vec3_make(-half_side, -half_side, -half_side) },
    { .pos = vec3_make(-half_side, half_side, -half_side) },
    { .pos = vec3_make(-half_side, -half_side, half_side) },
    { .pos = vec3_make(-half_side, half_side, half_side) },

    // Right side
    { .pos = vec3_make(half_side, -half_side, -half_side) },
    { .pos = vec3_make(half_side, half_side, -half_side) },
    { .pos = vec3_make(half_side, -half_side, half_side) },
    { .pos = vec3_make(half_side, half_side, half_side) },

    // Bottom side
    { .pos = vec3_make(-half_side, -half_side, -half_side) },
    { .pos = vec3_make(half_side, -half_side, -half_side) },
    { .pos = vec3_make(-half_side, -half_side, half_side) },
    { .pos = vec3_make(half_side, -half_side, half_side) },

    // Top side
    { .pos = vec3_make(-half_side, half_side, -half_side) },
    { .pos = vec3_make(half_side, half_side, -half_side) },
    { .pos = vec3_make(-half_side, half_side, half_side) },
    { .pos = vec3_make(half_side, half_side, half_side) },
  };

  const size_t indices[] = {
    0,  2,  1,  2,  3,  1,  4,  5,  7,  4,  7,  6,  8,  10, 9,  10, 11, 9,
    12, 13, 15, 12, 15, 14, 16, 17, 18, 18, 17, 19, 20, 23, 21, 20, 22, 23,
  };

  const size_t num_vertices = 24;
  const size_t num_indices = 36;

  SimpleMesh mesh = simple_mesh_make();
  simple_mesh_load_from_arrays(&mesh, vertices, num_vertices, indices, num_indices);
  return mesh;
}

static void vertex_shader(const Vertex* in, VSOut* out)
{
  Mat4 rotation = mat4_rotation_y(M_PI / 4);
  Mat4 translation = mat4_translation(0.0f, 0.0f, -2.0f);
  Mat4 transform = mat4_mul(&translation, &rotation);

  Vec4 in_pos = vec4_make(in->pos.x, in->pos.y, in->pos.z, 1.0f);
  out->pos = mat4_vec_mul(&transform, &in_pos);

  out->pos.x /= -out->pos.z;
  out->pos.y /= -out->pos.z;
}

void geom_shader(const VSOut* in0,
                 const VSOut* in1,
                 const VSOut* in2,
                 GSOut* out0,
                 GSOut* out1,
                 GSOut* out2,
                 size_t triangle_index)
{
  (void)triangle_index;
  out0->pos = in0->pos;
  out1->pos = in1->pos;
  out2->pos = in2->pos;
}

Color pixel_shader(const GSOut* in)
{
  (void)in;
  return 0xffffffff;
}
