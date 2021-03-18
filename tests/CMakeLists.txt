cmake_minimum_required(VERSION 3.5)
project(unittests LANGUAGES CXX)

find_package(Qt5Test REQUIRED)

include_directories(../src)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

enable_testing(true)

add_executable(unittests model/tst_line.cpp)
add_test(unittests unittests)

target_link_libraries(unittests Qt5::Test ${CMAKE_CURRENT_SOURCE_DIR}/../_build/release/libschimlib.a)

