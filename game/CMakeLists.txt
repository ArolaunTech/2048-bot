cmake_minimum_required(VERSION 3.25)

project(2048game LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE src
	"src/*.h"
	"src/*.cpp"
)

add_subdirectory("libs/glfw")

add_library (glad STATIC "libs/glad/src/glad.c")
target_include_directories(glad PUBLIC "libs/glad/include")

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(-Wall temp)

if(temp)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

check_cxx_compiler_flag(-O3 temp2)

if(temp2)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

add_executable(rungame ${src})
target_link_libraries(rungame glfw glad)
target_include_directories(rungame PUBLIC "libs/glad/include")