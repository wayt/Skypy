set(LIBRARY_NAME "shared")

add_definitions(
    -std=c++0x
)

file(GLOB source_file *.cpp *.h *.hpp)
file(GLOB serversock_file ServerSocket/*.cpp ServerSocket/*.h ServerSocket/*.hpp)
file(GLOB threading_file Threading/*.cpp Threading/*.h Threading/*.hpp)
file(GLOB database_file Database/*.cpp Database/*.h Database/*.hpp)
set(current_SRCS
    ${current_SRCS}
    ${source_file}
    ${serversock_file}
    ${threading_file}
    ${database_file}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ServerSocket
    ${CMAKE_CURRENT_SOURCE_DIR}/Threading
    ${CMAKE_CURRENT_SOURCE_DIR}/Database
    ${Boost_INCLUDE_DIRS}
    ${MYSQL_INCLUDE_DIR}
)

link_directories(
    ${Boost_LIBRARY_DIRS}
)

add_library(${LIBRARY_NAME}
    STATIC
    ${current_SRCS}
)
target_link_libraries(${LIBRARY_NAME}
    ${Boost_LIBRARIES}
    ${MYSQL_LIBRARIES}
)

