##############################################################################
# Project
project('linkmanager', 'cpp',
  version: '0.1.0',
  license: 'TBD',
  meson_version: '>=0.54.0',
  default_options: [
    'cpp_std=c++17',
    'default_library=static',
    'liberate:log_backend=spdlog',
  ])

##############################################################################
# Versioning, project and libraries

# There are so many versioning schemes, each of which is incompatible with
# others. We'll manage best by keeping things simple:
#
# - The package version follows semver
# - The library version is the package version
# - The ABI version, i.e. the compatibility we expect, is the major
#   component of the package
_splitver = meson.project_version().split('.')
PACKAGE_MAJOR = _splitver[0]
PACKAGE_MINOR = _splitver[1]
PACKAGE_PATCH = _splitver[2]

ABI_VERSION = PACKAGE_MAJOR
LIB_VERSION = meson.project_version()
PACKAGE_VERSION = meson.project_version()

##############################################################################
# Configuration

conf_data = configuration_data()
compiler = meson.get_compiler('cpp')

host_type = ''
if host_machine.system() in [ 'cygwin', 'darwin', 'dragonfly', 'freebsd', 'gnu', 'linux', 'netbsd' ]
  host_type = 'posix'
elif host_machine.system() == 'windows'
  host_type = 'win32'
elif host_machine.system().startswith('android')
  host_type = 'android'
endif
summary('Host platform', host_type, section: 'Platform')

# For Windows, try to determine the SDK version.
winsdk_ver = -1
if compiler.has_header('ntverp.h')
  code = '''#include <iostream>
#include <ntverp.h>

int main()
{
  std::cout << VER_PRODUCTMAJORVERSION;
}
'''
  result = compiler.run(code, name: 'Windows SDK version check.')
  winsdk_ver = result.stdout().to_int()
  summary('Windows SDK major version', winsdk_ver, section: 'Platform')
endif


### Compiler flags
compiler_id = compiler.get_id()

cpp_args = []

posix_common_args = [
  '-Wall', '-Wextra', '-Wpedantic', '-Wshadow', '-Wstrict-aliasing',
  '-Wstrict-overflow=5', '-Wcast-align',
  '-Wpointer-arith', '-Wcast-qual', '-Wold-style-cast',
]
if compiler_id == 'clang'
  cpp_args += posix_common_args + [
    '-Wabi',
  ]
elif compiler_id == 'gcc'
  cpp_args += posix_common_args + [
    '-Wstrict-null-sentinel',
  ]
elif compiler_id == 'msvc'
  cpp_args += [
    '/wd4250', '/wd4251', '/wd4275',
  ]
endif

bt = get_option('buildtype')
if bt in ['debug']
  if compiler_id == 'clang'
    cpp_args += ['-ggdb', '-DDEBUG']
  elif compiler_id == 'gcc'
    cpp_args += ['-g3', '-DDEBUG']
  elif compiler_id == 'msvc'
    cpp_args += ['/FS', '/DDEBUG=1']
  endif
elif bt in ['debugoptimized']
  if compiler_id == 'clang'
    cpp_args += ['-ggdb', '-DNDEBUG']
  elif compiler_id == 'gcc'
    cpp_args += ['-g3', '-DNDEBUG']
  elif compiler_id == 'msvc'
    cpp_args += ['/FS', '/DNDEBUG=1']
  endif
else
  posix_release_args = [
    '-fvisibility=hidden', '-fvisibility-inlines-hidden',
    '-DNDEBUG',
  ]
  if compiler_id == 'clang'
    cpp_args += posix_release_args
  elif compiler_id == 'gcc'
    cpp_args += posix_release_args
  elif compiler_id == 'msvc'
    cpp_args += ['/Oi', '/DNDEBUG=1']
  endif
endif

link_args = []
if host_type == 'android'
  # Only posix compilers supported (?)
  cpp_args += [
    '-DANDROID_STL=c++_shared',
    '-fexceptions', '-frtti',
  ]
  link_args = [
    '-lstdc++', '-lm',
  ]
endif


# Make things work with MSVC and Windows SDK <10
if compiler_id == 'msvc' and winsdk_ver < 10
  cpp_args += ['/permissive']
endif

add_project_arguments(cpp_args, language: 'cpp')


### Version and package information
conf_data.set_quoted('LINKMANAGER_PACKAGE_MAJOR', PACKAGE_MAJOR)
conf_data.set_quoted('LINKMANAGER_PACKAGE_MINOR', PACKAGE_MINOR)
conf_data.set_quoted('LINKMANAGER_PACKAGE_PATCH', PACKAGE_PATCH)
conf_data.set_quoted('LINKMANAGER_PACKAGE_VERSION', PACKAGE_VERSION)
conf_data.set_quoted('LINKMANAGER_ABI_VERSION', ABI_VERSION)
conf_data.set_quoted('LINKMANAGER_LIB_VERSION', LIB_VERSION)

conf_data.set_quoted('LINKMANAGER_PACKAGE_NAME', meson.project_name())
conf_data.set_quoted('LINKMANAGER_PACKAGE_URL', 'https://github.com/ocecelja/demo_code')


configure_file(
  input: 'build-config.h.in',
  output: 'build-config.h',
  configuration: conf_data)

dep_options = [
  'default_library=static',
]


##############################################################################
# Own subprojects
#multilink = subproject('multilink',
#  default_options: dep_options,
#)

##############################################################################
# Meson Wrap Dependencies
liberate = subproject('liberate',
  default_options: dep_options,
)
clipp = subproject('muellan-clipp',
  default_options: dep_options,
)
json = subproject('nlohmann_json',
  default_options: dep_options,
)


##############################################################################
# Project

install_headers(
  'include' / 'linkmanager' / 'api' / 'modules.h',

  subdir: 'linkmanager' / 'api'
)

install_headers(
  'include' / 'linkmanager' / 'api' / 'modules' / 'module.h',

  subdir: 'linkmanager' / 'api' / 'modules'
)

subdir('lib')
subdir('test')
subdir('src')
