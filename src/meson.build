##############################################################################
# Library

src = [
  'commands.cpp',
  'command' / 'lsmod.cpp',
  'command' / 'daemon.cpp',
  'command' / 'connection_manager.cpp',
  'main.cpp',
]

dronecomms = executable('linkmanager', src,
  link_args: link_args,
  dependencies: [
    linkmanager_internal,
    clipp.get_variable('clipp_dep'),
    liberate.get_variable('liberate_dep'),
    json.get_variable('nlohmann_json_dep'),
  ],
  install: true,
)
