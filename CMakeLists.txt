cmake_minimum_required(VERSION 3.0)

project(collections)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(.)

add_subdirectory(utils)
add_subdirectory(collections)
add_subdirectory(src)