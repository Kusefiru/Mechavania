cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)

project(Mechavania)

find_package(SDL2 CONFIG REQUIRED)
include_directories(src/ ${SDL2_INCLUDE_DIRS})

file(GLOB_RECURSE MECHA_SRC "src"
    "*.hpp"
    "*.cpp"
)

add_executable(Mechavania ${MECHA_SRC})
target_link_libraries(Mechavania PRIVATE SDL2::SDL2 SDL2::SDL2main)