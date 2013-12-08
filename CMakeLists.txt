# Project name
project(Skypy)

cmake_minimum_required(VERSION 2.6)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
if ( NOT CMAKE_BUILD_TYPE )
    SET(CMAKE_BUILD_TYPE Release)
endif()

set(DEBUG_FLAGS "")
if ( DEBUG )
    set(DEBUG_FLAGS "-ggdb3")
    SET(CMAKE_BUILD_TYPE Debug)
    message("-- Debug: yes")
else()
    message("-- Debug: no")
endif()

add_subdirectory(src)
