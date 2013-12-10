#find_package(Boost 1.46 COMPONENTS system thread-mt REQUIRED)

set(EXECUTABLE_NAME "server")

add_definitions(
    -std=c++0x
)

file(GLOB source_file *.cpp *.h *.hpp)
file(GLOB network_file Network/*.cpp Network/*.h Network/*.hpp)
set(current_SRCS
    ${current_SRCS}
    ${source_file}
    ${network_file}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Network
    ${CMAKE_SOURCE_DIR}/src/shared
    ${CMAKE_SOURCE_DIR}/src/shared/Database
    ${CMAKE_SOURCE_DIR}/src/shared/ServerSocket
    ${CMAKE_SOURCE_DIR}/src/shared/Threading
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
    shared
)
