cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)

project(deco)
conan_basic_setup()
include_directories(../include)

add_executable(test_parse parse.cpp)
conan_target_link_libraries(test_parse)

add_executable(test_serialize serialize.cpp)
conan_target_link_libraries(test_serialize)

add_executable(test_serialize_nested serialize_nested.cpp)
conan_target_link_libraries(test_serialize_nested)