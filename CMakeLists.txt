cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MilliganMichaelMinesweeper)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "C:/Users/CHawk/CLionProjects/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
add_executable(Minesweeper main.cpp Tile.cpp Tile.h Board.cpp Board.h TextureManager.cpp TextureManager.h Random.cpp Random.h Button.cpp Button.h Counter.cpp Counter.h)
target_link_libraries(Minesweeper sfml-graphics sfml-audio)