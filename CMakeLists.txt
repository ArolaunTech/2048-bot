cmake_minimum_required(VERSION 3.25)

project(2048 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_subdirectory("engine")
add_subdirectory("game")