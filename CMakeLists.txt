cmake_minimum_required(VERSION 3.25)

project(2048bot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

file(GLOB_RECURSE src
	"src/*.h"
	"src/*.cpp"
)

list(REMOVE_ITEM src "${CMAKE_SOURCE_DIR}/src/main.cpp" "${CMAKE_SOURCE_DIR}/src/train.cpp")

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(-Wall temp)

if(temp)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

check_cxx_compiler_flag(-O3 temp2)

if(temp2)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

add_executable(runbot ${src} src/main.cpp)
add_executable(trainbot ${src} src/train.cpp)