cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_NAME "Mechavania")

project(${PROJECT_NAME})

# SDL Linking and includes
find_package(SDL2 CONFIG REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
find_package(sdl2-image CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${MECHA_SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2_image)

add_subdirectory(src)
add_subdirectory(include)