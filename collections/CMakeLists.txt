cmake_minimum_required(VERSION 3.0)

aux_source_directory(./ libcollections)

ADD_DEFINITIONS(-DHAVE_QSORT_R_GNU)

ADD_DEFINITIONS(-DHAVE_QSORT_R)

set(CMAKE_C_FLAGS -g)

add_library(collection SHARED ${libcollections})

target_link_libraries(collection utils)
