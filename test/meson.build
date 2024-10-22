##############################################################################
# Tests

# Tests in a subproject are complicicated. You need all the compile and
# link flags from the enclosing project, etc.
if not meson.is_subproject()
  gtest = subproject('gtest')

  # See https://github.com/google/googletest/issues/813
  test_args = []
  if host_machine.system() == 'cygwin'
    test_args += ['-D_POSIX_C_SOURCE=200809L']
  endif

  # Google test issues this warning; disable it in *test* code only.
  if compiler_id == 'msvc'
    test_args = [
      '/wd4389',
    ]
  endif

  # Test plugin
  plugin_src = [
    'plugin.cpp',
  ]

  plugin = shared_module('test_plugin', plugin_src,
      dependencies: [
        linkmanager_internal,
      ],
      cpp_args: test_args,
  )

  # Unittests

  test_src = [
    'module_registry.cpp',
    'runner.cpp',
  ]

  # Additional tests only if -DDEBUG is given
  if bt in ['debug', 'debugoptimized']
    test_src += [
      # TODO?
    ]
  endif

endif
