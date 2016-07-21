include_directories(${CELERO_INCLUDE_DIR})

set(
        PERF_FILES
        benchmarks.cpp
        insert.cpp
        lookup.cpp
        iteration.cpp
        parameters.h
)
message(STATUS $ENV{CELERO_INCLUDE_DIR})
add_executable(benchmarks EXCLUDE_FROM_ALL ${PERF_FILES})
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