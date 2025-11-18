cmake_minimum_required(VERSION 3.14)
project(NorthEngine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include(CMake/DetectPlatform.cmake)

include(FetchContent)
include(CMake/FetchDeps.cmake)

add_subdirectory(Code/Modules)
add_subdirectory(Code/Sandbox)