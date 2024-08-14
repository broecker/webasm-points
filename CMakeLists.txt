project(WebGLPoints)

cmake_minimum_required(VERSION 3.5.1)

add_definitions(-std=c++11 -O3)

file(GLOB SOURCES src/*.cc)
include_directories(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
add_executable(a ${SOURCES})

message(STATUS "Setting compilation target to WASM")
set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
set_target_properties(a PROPERTIES LINK_FLAGS "-s MAX_WEBGL_VERSION=2 -s GL_ASSERTIONS -s WASM=1 -s EXPORTED_FUNCTIONS='[_main]'")
