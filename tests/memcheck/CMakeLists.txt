set(
        MEMCHECK_FILES
        memcheck.cpp
)

add_executable(memcheck ${MEMCHECK_FILES})
target_link_libraries(memcheck art)

add_custom_command(
        TARGET memcheck
        POST_BUILD
        COMMAND valgrind --tool=memcheck --track-origins=yes --leak-check=full
                --show-reachable=yes
                ${CMAKE_CURRENT_BINARY_DIR}/memcheck
)
