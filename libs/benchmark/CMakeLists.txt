include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add (
        benchmark
        GIT_REPOSITORY "https://github.com/google/benchmark.git"
        TIMEOUT 10
        CMAKE_ARGS "-DBENCHMARK_ENABLE_TESTING=OFF;-DCMAKE_BUILD_TYPE=Release"
        SOURCE_DIR "${CMAKE_BINARY_DIR}/benchmark"
        BINARY_DIR "${CMAKE_BINARY_DIR}/benchmark"
        INSTALL_COMMAND ""
        TEST_COMMAND ""
)
set_target_properties (benchmark PROPERTIES EXCLUDE_FROM_ALL TRUE)
set (GBENCHMARK_FOUND true)
ExternalProject_Get_Property(benchmark source_dir)
set (GBENCHMARK_INCLUDE_DIR "${source_dir}/include" CACHE INTERNAL "Path to include folder for benchmark")
ExternalProject_Get_Property(benchmark binary_dir)
set (GBENCHMARK_LIBRARY "${binary_dir}/src/libbenchmark.a" CACHE INTERNAL "Path to binary folder for benchmark")