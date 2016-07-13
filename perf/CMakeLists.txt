include_directories(${CELERO_INCLUDE_DIR})
enable_testing(true)

set(
        PERF_FILES
        perf.cpp
)

add_executable(benchmarks ${PERF_FILES})
target_link_libraries(
        benchmarks
        art
        ${CELERO_LIBRARY}
)
add_dependencies(
        benchmarks
        art
        Celero
)