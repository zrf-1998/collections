cmake_minimum_required(VERSION 3.0)

aux_source_directory(./ libutils)

set(CMAKE_C_FLAGS -g)

add_library(utils SHARED ${libutils})