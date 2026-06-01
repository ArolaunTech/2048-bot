cmake_minimum_required(VERSION 3.25)

project(2048game LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE src
	"src/*.h"
	"src/*.cpp"
)

include(FetchContent)

FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG cf8edaa020cfd6769f713a18d93a7b117166e3df
)

FetchContent_MakeAvailable(cli11)

cmake_policy(SET CMP0167 OLD)
find_package(Boost 1.90.0 REQUIRED COMPONENTS process)

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
target_link_libraries(rungame CLI11::CLI11)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(rungame ${Boost_LIBRARIES})
endif()