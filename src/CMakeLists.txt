cmake_minimum_required(VERSION 3.0)

include_directories(../inc)

aux_source_directory(./ src)

set(CMAKE_C_FLAGS -g)

add_compile_options(-Wall -O0)

link_directories(.)

add_executable(main ${src})

target_link_libraries(main collection)