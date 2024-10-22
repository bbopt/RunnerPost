# Application modules

#
# Algos
#
set(HEADERS
AlgoParameters.hpp
Problem.hpp
Result.hpp
Runner.hpp
runner_defines.hpp
)

set(SOURCES
AlgoParameters.cpp
main.cpp
Problem.cpp
Result.cpp
Runner.cpp
)



#
# Generate Attribute headers
#


#
# Build libraries and executable
# Install all
#

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# export all symbols on Windows
if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Build shared libraries (runnerPost)
include(./CMakeListsLibs.txt)

# build statically linked library (runnerPostStatic) for Python interface
if(BUILD_INTERFACE_PYTHON MATCHES ON)
   include(./CMakeListsLibsStatic.txt)
endif()

# Build dynamically linked executables (nomad)
include(./CMakeListsExe.txt)

# Install header files
install(
  FILES ${HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/runnerpost_nsbegin.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/runnerpost_nsend.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/runnerpost_platform.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/runnerpost_version.hpp
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
)
