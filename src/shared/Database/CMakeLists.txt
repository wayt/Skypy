# Project name
project(test_mysql)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(Boost 1.46 COMPONENTS system REQUIRED)
find_package(MySQL REQUIRED)

cmake_minimum_required(VERSION 2.6)
if ( NOT CMAKE_BUILD_TYPE )
    SET(CMAKE_BUILD_TYPE Release)
endif()

set(EXECUTABLE_NAME "test_mysql")
set(CMAKE_CXX_FLAGS "-std=c++0x -g ${CMAKE_CXX_FLAGS}")

file(GLOB source_file *.cpp *.h *.hpp)
set(current_SRCS
    ${current_SRCS}
    ${source_file}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${MYSQL_INCLUDE_DIR}
)

link_directories(
    ${Boost_LIBRARY_DIRS}
)

add_executable(${EXECUTABLE_NAME}
    ${current_SRCS})

target_link_libraries(
    ${EXECUTABLE_NAME}
    ${Boost_SYSTEM_LIBRARY}
    ${MYSQL_LIBRARIES}
)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")

install_targets(/ ${EXECUTABLE_NAME})
