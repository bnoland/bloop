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

sources = []
subdir('resources')
subdir('src')

executable(
  meson.project_name(),
  sources: sources,
  dependencies: [m_dep, sdl2_dep],
  include_directories: ['src'],
)
