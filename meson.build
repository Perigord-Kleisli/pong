project('pong', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

exe = executable('pong', './src/Main.c',
  dependencies: 
    [
      dependency('sdl2', version: '>=2.26.5'),
      dependency('SDL2_mixer', version: '>=1.2.12')
    ]
  )

test('basic', exe)
