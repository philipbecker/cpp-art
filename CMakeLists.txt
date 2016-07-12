cmake_minimum_required(VERSION 3.5)
project(ART)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS_DEBUG "-DMY_DEBUG")
set(SOURCE_FILES adapt_radix_tree.h art_map.h)

set(TEST_FILES
        tests/test.cpp tests/art_basic.cpp tests/art_stress.cpp
        tests/art_node_4.cpp tests/art_node_16.cpp
        tests/art_node_48.cpp tests/art_iterator.cpp
        )
add_executable(ART ${SOURCE_FILES})
add_executable(ART_MAIN ${SOURCE_FILES} main.cpp)
add_executable(TEST_ART ${SOURCE_FILES} libs/catch.h  ${TEST_FILES} )