project(
  'bloop',
  ['c'],
  version: '0.1',
  default_options: ['warning_level=3', 'c_std=c17'],
)

if get_option('buildtype') == 'debug'
  add_project_arguments('-fno-inline', language: ['c'])
  add_project_arguments('-DDEBUG', language: ['c'])
endif

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required: false)

sdl2_dep = dependency('sdl2')

subdir('resources')

executable(
  meson.project_name(),
  sources: [
    'src/main.c',
    'src/graphics.c',
    'src/vector.c',
    'src/matrix.c',
    'src/model.c',
    'src/dynlist.c',
    'src/utility.c',
    'src/effects/default_effect.c',
    'src/effects/color_effect.c',
    'src/pipelines/color_pipeline.c',
    'src/pipelines/default_pipeline.c',
    'src/meshes/default_mesh.c',
    'src/meshes/color_mesh.c',
    'src/scenes/color_cube_scene.c',
    'src/scenes/solid_cube_scene.c',
  ],
  dependencies: [m_dep, sdl2_dep],
  include_directories: ['src'],
)
