cmake_minimum_required(VERSION 3.5)
project(ART)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS_DEBUG "-DMY_DEBUG")
set(SOURCE_FILES
        _node.h _leaf.h node_4.h node_16.h
        node_48.h node_256.h adapt_radix_tree.h )

set(TEST_FILES
        tests/test.cpp tests/art_basic.cpp tests/art_stress.cpp
        tests/art_node_4.cpp tests/art_node_16.cpp
        tests/art_node_48.cpp
        )
add_executable(ART ${SOURCE_FILES})
add_executable(ART_MAIN ${SOURCE_FILES} main.cpp)
add_executable(TEST_ART ${SOURCE_FILES} libs/catch.h  ${TEST_FILES} )