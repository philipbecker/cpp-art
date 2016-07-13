include_directories(${CATCH_INCLUDE_DIR})
enable_testing(true)

set(
        TEST_FILES
        test.cpp
        art_basic.cpp
        art_stress.cpp
        art_node_4.cpp
        art_node_16.cpp
        art_node_48.cpp
        art_iterator.cpp
)

add_executable(tests ${TEST_FILES})
target_link_libraries(tests art)
add_dependencies(tests catch)