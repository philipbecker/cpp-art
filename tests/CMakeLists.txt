include_directories(${CATCH_INCLUDE_DIR})
enable_testing(true)

set(
        TEST_FILES
        test.cpp
        art_basic.cpp
        art_node_4.cpp
        art_node_16.cpp
        art_node_48.cpp
        radix_map/iterator.cpp
        radix_map/move_and_copy.cpp
        radix_map/relational_operators.cpp
        radix_map/value_comp.cpp
)

add_executable(tests ${TEST_FILES})
target_link_libraries(tests art)
add_dependencies(tests catch)

add_executable(stress_tests ${TEST_FILES} radix_map/stress_tests.cpp)
target_link_libraries(stress_tests art)
add_dependencies(stress_tests catch)

add_subdirectory(memcheck)
