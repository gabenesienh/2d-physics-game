cmake_minimum_required(VERSION 3.28)
project(2d-physics-game)

set(CMAKE_CXX_STANDARD           17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS        OFF)

add_executable(${PROJECT_NAME}
	src/events.cpp
	src/game.cpp
	src/graphics.cpp
	src/levels.cpp
	src/main.cpp
	src/objects.cpp
	src/preferences.cpp
	src/quadtree.cpp
	src/tiles.cpp
	src/util.cpp
)

find_package(SDL2 REQUIRED COMPONENTS SDL2)

if(TARGET SDL2::SDL2main)
	target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)