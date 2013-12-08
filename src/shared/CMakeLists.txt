# Project name
project(test_network)

find_package(Boost 1.46 COMPONENTS system thread-mt REQUIRED)

cmake_minimum_required(VERSION 2.6)
if ( NOT CMAKE_BUILD_TYPE )
    SET(CMAKE_BUILD_TYPE Release)
endif()

set(EXECUTABLE_NAME "test_network")

add_definitions(
    -std=c++11
)

file(GLOB source_file *.cpp *.h *.hpp)
file(GLOB serversock_file ServerSocket/*.cpp ServerSocket/*.h ServerSocket/*.hpp)
file(GLOB threading_file Threading/*.cpp Threading/*.h Threading/*.hpp)
set(current_SRCS
    ${current_SRCS}
    ${source_file}
    ${serversock_file}
    ${threading_file}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ServerSocket
    ${CMAKE_CURRENT_SOURCE_DIR}/Threading
    ${Boost_INCLUDE_DIRS}
)

link_directories(
    ${Boost_LIBRARY_DIRS}
)

add_executable(${EXECUTABLE_NAME}
    ${current_SRCS}
)
target_link_libraries(${EXECUTABLE_NAME}
    ${Boost_LIBRARIES}
)

install_targets(/ ${EXECUTABLE_NAME})
