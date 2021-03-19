cmake_minimum_required(VERSION 3.5)
project(unittests LANGUAGES CXX)

# Configuration
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

# Variables
set(QT_VERSION_MAJOR 5)
set(QT_VERSION 5.9)
set(EXECUTABLE ${CMAKE_PROJECT_NAME})
set(SRC_DIR ${CMAKE_SOURCE_DIR}/../src)

# Find QtTest library
find_package(Qt5 5.9 COMPONENTS Test REQUIRED)

include_directories(${SRC_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include(${SRC_DIR}/common.cmake)
target_sources(${EXECUTABLE} PRIVATE
    model/tst_line.cpp
)

enable_testing(true)
add_test(${EXECUTABLE} ${EXECUTABLE})

target_link_libraries(${EXECUTABLE} Qt5::Test)
