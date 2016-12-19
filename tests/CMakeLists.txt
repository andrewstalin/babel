cmake_minimum_required(VERSION 3.2)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)
project(babel.tests)

include(GetProjectFiles)
include(SetCompilerOptions)

set(BABEL_TESTS_FILES)
get_project_files(BABEL_TESTS_FILES)

add_executable(${PROJECT_NAME} ${BABEL_TESTS_FILES})
set_compiler_options()

set_property(TARGET babel.tests APPEND PROPERTY INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/babel/include
    ${CMAKE_SOURCE_DIR}/3rdparty/gtest/googletest/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE gtest)
target_link_libraries(${PROJECT_NAME} PRIVATE babel)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE iconv)
endif()

