# ----------------------------------------------------------------------------
#  Root CMake file for RunnerPost
# ----------------------------------------------------------------------------

# cmake version : the latest one
cmake_minimum_required(VERSION 3.15...3.15)

set(RUNNERPOST_VERSION_MAJOR 1)
set(RUNNERPOST_VERSION_MINOR 0)

set(RUNNERPOST_VERSION ${RUNNERPOST_VERSION_MAJOR}.${RUNNERPOST_VERSION_MINOR})

# name of the project
# Need to update when version changes
project (RunnerPost VERSION ${RUNNERPOST_VERSION_MAJOR}.${RUNNERPOST_VERSION_MINOR})

# use standard compilers parameters for c++14
SET(CMAKE_CXX_STANDARD 14 )
SET(CMAKE_CXX_STANDARD_REQUIRED ON )


#check compiler version
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8)
        message(FATAL_ERROR "GCC version < 8 has not been tested for Nomad")
    endif()
elseif (("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") 
    OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    # require at least clang 5
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5)
        message(FATAL_ERROR "Clang version has not been tested for Nomad")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # require at least 15.0 (MSVC 2017) for C++14 support
    if (MSVC_TOOLSET_VERSION VERSION_LESS 141)
        message(FATAL_ERROR "MSVC version ${CMAKE_CXX_COMPILER_VERSION} has not been tested for Nomad")
    endif()
else()
    message(WARNING "You are using an unsupported compiler. Compilation has only been tested with Clang, GCC, and MSVC.")
endif()

# Load some modules
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# special flag required
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#
# Message for starting configuration
#
message(CHECK_START " Configuring custom options")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

#
# Modify the install prefix if not specified on the command
#
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
   set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR} CACHE PATH "..." FORCE) 
endif()
message(STATUS "  Installation prefix set to ${CMAKE_INSTALL_PREFIX}")

#
# Choose to build with unit tests or not
# Other tests are performed on examples based on the BUILD_TESTS flag
#
option(BUILD_TESTS "Option to build the unit tests" OFF)
if(BUILD_TESTS MATCHES ON)
   message(CHECK_START "  Configuring unit tests build")

   if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
        message(FATAL_ERROR "The unit tests are not available. ")
   endif()
     
   include(FetchContent)

   # Download Googletest
   FetchContent_Declare(
       googletest
       GIT_REPOSITORY https://github.com/google/googletest.git
       GIT_TAG        v1.14.0
       )

   # Make the library available
   FetchContent_MakeAvailable(googletest)

   enable_testing()

   add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
   message(CHECK_PASS " done")
else()
   message(STATUS "  Unit tests NOT built")
endif()

#
# Test openMP package
#
option(ENABLE_OPENMP "Option to build with OpenMP" ON)
if(ENABLE_OPENMP MATCHES ON)
   find_package(OpenMP QUIET)
   if(OpenMP_FOUND)
      message(STATUS "  Test OpenMP for parallel functionalities  -- found ")
   else()
      message(STATUS "  OpenMP not available. Forcefully disabled")
   endif()
else()
    message(STATUS "  OpenMP disabled")
endif()


#
# Choose to build the Python interface
#
option(BUILD_INTERFACE_PYTHON "Option to build Python interface" OFF)
if(BUILD_INTERFACE_PYTHON MATCHES ON)
   set(Python3_FIND_VIRTUALENV "First") # Using virtualenv to have cython and wheel is easy
   find_package(Python 3.6 QUIET REQUIRED)
   message(CHECK_START "  Configuring build for Python interface (Python ${Python_VERSION})")
   add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pythonInterface)
   message(CHECK_PASS " done")
else()
   message(STATUS "  Python interface NOT built")
endif()


#
# Custom options final message
#
list(REMOVE_ITEM CMAKE_MESSAGE_INDENT "  ")
message(CHECK_PASS " done")


#
# Add RunnerPost directory for building 
#
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
