cmake_minimum_required(VERSION 3.5)
project(reference)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS_DEBUG "-DMY_DEBUG")
set(SOURCE_FILES
        Node.h Leaf.h Node4.h Node16.h
        Node48.h Node256.h adapt_radix_tree.h )

set(TEST_FILES
        tests/test.cpp tests/art_basic.cpp tests/art_stress.cpp
        tests/art_node_4.cpp tests/art_node_16.cpp
        )
add_executable(ART ${SOURCE_FILES})
add_executable(ART_MAIN main.cpp ${SOURCE_FILES})
add_executable(TEST_ART libs/catch.h ${SOURCE_FILES} ${TEST_FILES})