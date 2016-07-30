include_directories(${CATCH_INCLUDE_DIR})
enable_testing(true)

set(
        TEST_FILES
        test.cpp
        art/growing_and_tiebreaking.cpp
        art/node_4.cpp
        art/node_16.cpp
        art/node_48.cpp
        radix_map/capacity.cpp
        radix_map/modification.cpp
        radix_map/iterator.cpp
        radix_map/move_and_copy.cpp
        radix_map/relational_operators.cpp
        radix_map/value_comp.cpp
        radix_map/stress_tests.cpp
        radix_set/modification.cpp
        radix_set/iterator.cpp
        radix_set/stress_tests.cpp
        radix_set/move_and_copy.cpp
)

add_executable(tests ${TEST_FILES})
target_link_libraries(tests art)
add_dependencies(tests catch)

add_subdirectory(memcheck)
