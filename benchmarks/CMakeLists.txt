include_directories(${CELERO_INCLUDE_DIR})
include_directories(${GBENCHMARK_INCLUDE_DIR})

# CELERO BENCHMARK SUITE
set(
        PERF_FILES
        celero/benchmarks.cpp
        celero/insert.cpp
        celero/lookup.cpp
        celero/iteration.cpp
        celero/parameters.h
)

add_executable(celeros_suite EXCLUDE_FROM_ALL ${PERF_FILES})

target_link_libraries(
        celeros_suite
        art
        ${CELERO_LIBRARY}
)

add_dependencies(
        celeros_suite
        art
        Celero
)

# GOOGLE BENCHMARK SUITE
set(
        GBENCH_FILES
        gbench/gbenchmarks.cpp
)

add_executable(gbench_suite EXCLUDE_FROM_ALL ${GBENCH_FILES})

target_link_libraries(
        gbench_suite
        art
        ${GBENCHMARK_LIBRARY}
        pthread
)

add_dependencies(
        gbench_suite
        art
        benchmark
)

# MEMORY USAGE
set(
        MEM_FILES
        memusage/memusage.cpp
)
add_executable(memusage EXCLUDE_FROM_ALL ${MEM_FILES})
target_link_libraries(memusage art)
add_dependencies(memusage art)