set(RUNNERPOST_SOURCE
${CMAKE_CURRENT_SOURCE_DIR}/RunnerPostCythonInterface.cpp
${CMAKE_CURRENT_SOURCE_DIR}/runnerPost.pyx
${CMAKE_CURRENT_SOURCE_DIR}/setup.py
)


# Modern-ish approach that does not (openly) complain about being deprecated.
# The build process supports both GCC and MSVC.

add_custom_target(
    PyRunnerPost ALL
    COMMAND
    ${CMAKE_COMMAND} -E env "RUNNERPOST_SRC=${CMAKE_SOURCE_DIR}/src" "RUNNERPOST_BUILD_DIR=${CMAKE_BINARY_DIR}" "RUNNERPOST_MSVC_FLAG=${MSVC}" "RUNNERPOST_MSVC_CONF=$<CONFIG>" ${Python_EXECUTABLE} -m pip wheel -w dist -- .
    DEPENDS 
      runnerPostStatic
      ${RUNNERPOST_SOURCE}
    WORKING_DIRECTORY 
      ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM
)
