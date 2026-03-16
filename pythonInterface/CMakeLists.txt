set(RUNNERPOST_SOURCE
${CMAKE_CURRENT_SOURCE_DIR}/RunnerPostCythonInterface.cpp
${CMAKE_CURRENT_SOURCE_DIR}/runnerPost.pyx
${CMAKE_CURRENT_SOURCE_DIR}/setup.py
)

# Detect sysroot for macOS, otherwise leave empty
if(APPLE)
    set(RUNNERPOST_SYSROOT ${CMAKE_OSX_SYSROOT})
else()
    set(RUNNERPOST_SYSROOT "")
endif()

# Detect architecture 
if(CMAKE_OSX_ARCHITECTURES)
    set(PY_ARCH ${CMAKE_OSX_ARCHITECTURES})
elseif(CMAKE_SYSTEM_PROCESSOR)
    set(PY_ARCH ${CMAKE_SYSTEM_PROCESSOR})
else()
    set(PY_ARCH "unknown")
endif()


# Modern-ish approach that does not (openly) complain about being deprecated.
# The build process supports both GCC and MSVC.

add_custom_target(
    PyRunnerPost ALL
    COMMAND
    ${CMAKE_COMMAND} -E env 
     	 "CC=${CMAKE_C_COMPILER}"
      	"CXX=${CMAKE_CXX_COMPILER}"
      	"SYSROOT=${RUNNERPOST_SYSROOT}"
	"RUNNERPOST_SRC=${CMAKE_SOURCE_DIR}/src" 
	"RUNNERPOST_BUILD_DIR=${CMAKE_BINARY_DIR}" 
	"RUNNERPOST_MSVC_FLAG=${MSVC}" 
	"RUNNERPOST_MSVC_CONF=$<CONFIG>" 
	ARCHFLAGS=-arch\ ${PY_ARCH}
	${Python_EXECUTABLE} -m pip wheel -w dist -- .
    DEPENDS 
      runnerPostStatic
      ${RUNNERPOST_SOURCE}
    WORKING_DIRECTORY 
      ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM
)
