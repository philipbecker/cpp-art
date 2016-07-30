include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
        Celero
        PREFIX ${CMAKE_BINARY_DIR}/Celero
        GIT_REPOSITORY "https://github.com/DigitalInBlue/Celero.git"
        TIMEOUT 10
        CMAKE_ARGS "-DCELERO_ENABLE_EXPERIMENTS=OFF;-DCMAKE_BUILD_TYPE=Release"
        SOURCE_DIR "Celero"
        BINARY_DIR "CeleroBuild"
        INSTALL_COMMAND ""
        TEST_COMMAND ""
)
set_target_properties(Celero PROPERTIES EXCLUDE_FROM_ALL TRUE)
set(CELERO_FOUND true)
ExternalProject_Get_Property(Celero source_dir)
set(CELERO_INCLUDE_DIR "${source_dir}/include" CACHE INTERNAL "Path to include folder for Celero")
ExternalProject_Get_Property(Celero binary_dir)
set(CELERO_LIBRARY "${binary_dir}/libcelero.so" CACHE INTERNAL "Path to binary folder for Celero")