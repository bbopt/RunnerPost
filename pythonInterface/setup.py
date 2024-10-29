#!/usr/bin/env python3
#
# Support for statically linked Python interface.

# Environment variables are used for configuration.
#
# (1) RUNNERPOST_SRC for source directory.
# (2) RUNNERPOST_BUILD for build directory.
# (3) RUNNERPOST_MSVC_FLAG for signalization that MSVC will be used.
# (4) RUNNERPOST_MSVC_CONF for signalization of MSVC build configuration.
#
# Export the variables. 

import setuptools 
from Cython.Build import cythonize

import sys
import os.path
import os
import re

# Environment processing

env_runnerpost_src = os.environ.get('RUNNERPOST_SRC')
env_runnerpost_build_dir = os.environ.get('RUNNERPOST_BUILD_DIR')
env_runnerpost_msvc_flag = os.environ.get('RUNNERPOST_MSVC_FLAG')
env_runnerpost_msvc_conf = os.environ.get('RUNNERPOST_MSVC_CONF')

if not(env_runnerpost_src):
    print('Missing RUNNERPOST_SRC env.')
    sys.exit(1)
    
if not(env_runnerpost_build_dir):
    print('Missing RUNNERPOST_BUILD_DIR env.')
    sys.exit(1)

if not(env_runnerpost_msvc_flag):
    print('Missing RUNNERPOST_MSVC_FLAG, assuming GCC compatible compiler.')

if not(env_runnerpost_msvc_conf):
    print('Missing RUNNERPOST_MSVC_CONF, assuming Release configuration.')

# Construct base paths

path_include = env_runnerpost_src
path_library_runnerpost = os.path.join(env_runnerpost_build_dir, 'src')

# Compiler and linker configuration
setup_compile_args = []
if env_runnerpost_msvc_flag:
    setup_compile_args.append('/std:c++14')
else:
    setup_compile_args.append('-w')
    setup_compile_args.append('-std=c++14')
    setup_compile_args.append('-pthread')


# MSVC linker automagically resolves static libraries
# by their base names if given appropriate search paths.
#
# Hence setup_libraries and setup_library_dirs.

setup_libraries = []
setup_library_dirs = []

# GCC compatible linkers can be explicitly instructed to use 
# static libraries as well (-l:libname.a). It is also possible
# to provide the static libraries directly as extra objects
# for the linker.
# 
# Hence setup_extra_objects.

setup_extra_objects = []

if env_runnerpost_msvc_flag:
    setup_libraries.append('runnerPostStatic')

    setup_library_dirs.append(os.path.join(path_library_runnerpost, env_runnerpost_msvc_conf))

else:
    setup_extra_objects.append(os.path.join(path_library_runnerpost, 'librunnerPostStatic.a'))
    
# Determine package version directly from header file.
__version__ = "unknown"
path_version = os.path.join(path_include, 'runnerpost_version.hpp')

with open(path_version, encoding = 'utf-8') as file:
    pattern = '#define\s+RUNNERPOST_VERSION_NUMBER\s+"([^"]+)"'
    for line in file:
        if (match := re.match(pattern, line)):
            __version__ = match.group(1)
            break

# Off we go.

setuptools.setup(
    name = 'RunnerPost',
    version = __version__,
    author = 'Christophe Tribes',
    author_email = 'christophe.tribes@polymtl.ca',
    description = 'RunnerPost Python interface for optimization results post processing and profiling',
    url = 'https://github.com/bbopt/RunnerPost',
    ext_modules = cythonize(setuptools.Extension(
        'RunnerPost',
        sources = [ 'RunnerPost.pyx' ],
        include_dirs = [ path_include ],
        extra_compile_args = setup_compile_args,
        extra_objects = setup_extra_objects,
        libraries = setup_libraries,
        library_dirs = setup_library_dirs,
        language  =  'c++'
    )),
)

