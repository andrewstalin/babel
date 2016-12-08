cmake_minimum_required(VERSION 3.2)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)
project(babel)

include(GetProjectFiles)
include(SetCompilerOptions)

set(BABEL_FILES)
get_project_files(BABEL_FILES)

add_library(babel STATIC ${BABEL_FILES})
set_compiler_options()

set_property(TARGET babel APPEND PROPERTY INCLUDE_DIRECTORIES
    ${babel_SOURCE_DIR}/include
    ${babel_SOURCE_DIR}/src
)