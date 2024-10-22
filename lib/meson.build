##############################################################################
# Library

includes = include_directories('..' / 'include')

lib_src = [
  'module_registry.cpp',
]

libincludes = [
  include_directories('.'),
  include_directories('..'),
]

dl_dep = compiler.find_library('dl', required: true)

liblinkmanager = static_library('linkmanager', lib_src,
  include_directories: [includes, libincludes],
  # version: LIB_VERSION,
  # soversion: ABI_VERSION,
  dependencies: [
    dl_dep,
    liberate.get_variable('liberate_dep'),
    json.get_variable('nlohmann_json_dep'),
  ],
  link_args: link_args,
  install: true,
)

linkmanager_internal = declare_dependency(
  include_directories: [includes, libincludes],
  dependencies: [
    liberate.get_variable('liberate_dep'),
    json.get_variable('nlohmann_json_dep'),
  ],
  link_with: [liblinkmanager],
  link_args: link_args,
  version: LIB_VERSION,
)
