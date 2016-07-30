cmake_minimum_required(VERSION 2.8)
project(ART)

set(EXT_PROJECTS_DIR libs)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-DMY_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

set(ART_INCLUDE_DIR include)
set(
        SOURCE_FILES
        include/art/key_transform.h
        include/art/adaptive_radix_tree.h
        include/art/radix_map.h
        include/art/radix_set.h
)

add_library(art STATIC ${SOURCE_FILES})
set_target_properties(art PROPERTIES LINKER_LANGUAGE CXX)

include_directories(${ART_INCLUDE_DIR})

set(BTREE_INCLUDE_DIR ${EXT_PROJECTS_DIR}/cpp-btree)
include_directories(${BTREE_INCLUDE_DIR})

# TESTS
add_subdirectory(${EXT_PROJECTS_DIR}/catch)
add_subdirectory(tests)

# BENCHMARKS
add_subdirectory(${EXT_PROJECTS_DIR}/celero)
add_subdirectory(${EXT_PROJECTS_DIR}/benchmark)
add_subdirectory(benchmarks)

# CLION needs this
include_directories(${CATCH_INCLUDE_DIR})
include_directories(${CELERO_INCLUDE_DIR})
include_directories(${GBENCHMARK_INCLUDE_DIR})
