cmake_minimum_required(VERSION 2.6.0)
project(Life C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -pedantic")

set(BIN_DIR ${Life_SOURCE_DIR}/bin)
include_directories(/usr/include/SDL2/ src/)

FILE(GLOB_RECURSE SRC_FILES ${Life_SOURCE_DIR}/src/*.c)
add_executable(life ${SRC_FILES})

target_link_libraries(life SDL2)

install(DIRECTORY ${Life_SOURCE_DIR}/assets/ DESTINATION ${BIN_DIR}/assets)
install(TARGETS life RUNTIME DESTINATION ${BIN_DIR})
