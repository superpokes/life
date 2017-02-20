cmake_minimum_required(VERSION 2.6.0)
project(Life C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -pedantic")

set(BIN_DIR ${Life_SOURCE_DIR}/bin)
include_directories(/usr/include/SDL2/ src/)
add_executable(life.out src/life.c src/world.c src/life_gui.c)
target_link_libraries(life.out SDL2)

install(TARGETS life.out RUNTIME DESTINATION ${BIN_DIR})
