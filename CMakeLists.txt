cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(OIGenerator VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_CXX_FLAGS -g -Wall)
# string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

enable_testing()

add_subdirectory(third_party/catch2 EXCLUDE_FROM_ALL)
add_subdirectory(third_party/fmt EXCLUDE_FROM_ALL)
add_subdirectory(third_party/filesystem EXCLUDE_FROM_ALL)
add_subdirectory(third_party/cxxopts EXCLUDE_FROM_ALL)
add_subdirectory(src)
add_subdirectory(tests)
