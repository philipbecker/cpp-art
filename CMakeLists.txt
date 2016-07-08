cmake_minimum_required(VERSION 3.5)
project(reference)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS_DEBUG "-DMY_DEBUG")
set(SOURCE_FILES
        Node.cpp Node.h Node4.cpp Node4.h Node16.cpp Node16.h
        Node48.cpp Node48.h Node256.cpp Node256.h
        adapt_radix_tree.cpp adapt_radix_tree.h Leaf.cpp Leaf.h)
add_executable(ART ${SOURCE_FILES})
add_executable(ART_MAIN main.cpp ${SOURCE_FILES})
add_executable(TEST_ART libs/catch.h tests/test.cpp ${SOURCE_FILES})