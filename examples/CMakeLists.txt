cmake_minimum_required(VERSION 3.29)
project(examples)

set(CMAKE_CXX_STANDARD 20)

find_package(ZLIB REQUIRED)
find_package(tinyvtu REQUIRED)

add_executable(example_tetra example_tetra.cpp)

target_link_libraries(example_tetra tinyvtu::tinyvtu ZLIB::ZLIB)