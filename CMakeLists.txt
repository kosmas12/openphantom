cmake_minimum_required(VERSION 3.16)
project(openphantom C)

set(CMAKE_C_STANDARD 11)

FIND_PACKAGE(SDL2 REQUIRED)

add_executable(openphantom main.c include/openphantom.c)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(openphantom ${SDL2_LIBRARIES})

configure_file(${CMAKE_SOURCE_DIR}/openphantom.bmp ${CMAKE_BINARY_DIR}/openphantom.bmp COPYONLY)
