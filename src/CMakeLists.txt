cmake_minimum_required(VERSION 3.5)

if(DEFINED ENV{VERSION})
    project(schim VERSION $ENV{VERSION} LANGUAGES CXX)
else()
    project(schim LANGUAGES CXX)
endif()

# Variables
set(QT_VERSION_MAJOR 5)
set(QT_VERSION 5.9)
set(EXECUTABLE schim)
set(SRC_DIR ${CMAKE_SOURCE_DIR})

include(common.cmake)
target_sources(${EXECUTABLE} PRIVATE main.cpp)

