cmake_minimum_required(VERSION 3.4)
project(ART)

set(EXT_PROJECTS_DIR libs)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-DMY_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

set(ART_INCLUDE_DIR include)
set(
        SOURCE_FILES
        include/art/key_transform.h
        include/art/adaptive_radix_tree.h
        include/art/radix_map.h
)

add_library(art STATIC ${SOURCE_FILES})
set_target_properties(art PROPERTIES LINKER_LANGUAGE CXX)

# CLION needs this
include_directories(${ART_INCLUDE_DIR})
include_directories(${CATCH_INCLUDE_DIR})
include_directories(${CELERO_INCLUDE_DIR})

# TESTS
add_subdirectory(${EXT_PROJECTS_DIR}/catch)
add_subdirectory(tests)

# BENCHMARKS
add_subdirectory(${EXT_PROJECTS_DIR}/celero)
add_subdirectory(benchmarks)

# SANDBOX MAIN
add_executable(main ${SOURCE_FILES} main.cpp)
target_link_libraries(main art)
add_dependencies(main art)

add_custom_target(full)
add_dependencies(
        full
        main
        tests
        benchmarks
)