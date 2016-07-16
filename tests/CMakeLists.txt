include_directories(${CATCH_INCLUDE_DIR})
enable_testing(true)

set(
        TEST_FILES
        test.cpp
        art_basic.cpp
        art_node_4.cpp
        art_node_16.cpp
        art_node_48.cpp
        art_iterator.cpp
)

add_executable(tests ${TEST_FILES})
target_link_libraries(tests art)
add_dependencies(tests catch)

add_executable(stress_tests ${TEST_FILES} art_stress.cpp)
target_link_libraries(stress_tests art)
add_dependencies(stress_tests catch)

add_subdirectory(memcheck)
