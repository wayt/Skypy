set(EXECUTABLE_NAME "skypy-server")

add_definitions(
    -std=c++0x
)

file(GLOB source_file *.cpp *.h *.hpp)
file(GLOB network_file Network/*.cpp Network/*.h Network/*.hpp)
file(GLOB session_file Session/*.cpp Session/*.h Session/*.hpp)
file(GLOB config_file Config/*.cpp Config/*.h Config/*.hpp)
file(GLOB database_file Database/*.cpp Database/*.h Database/*.hpp)
set(current_SRCS
    ${current_SRCS}
    ${source_file}
    ${network_file}
    ${session_file}
    ${config_file}
    ${database_file}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Network
    ${CMAKE_CURRENT_SOURCE_DIR}/Session
    ${CMAKE_CURRENT_SOURCE_DIR}/Config
    ${CMAKE_CURRENT_SOURCE_DIR}/Database
    ${CMAKE_SOURCE_DIR}/src/shared
    ${CMAKE_SOURCE_DIR}/src/shared/Database
    ${CMAKE_SOURCE_DIR}/src/shared/ServerSocket
    ${CMAKE_SOURCE_DIR}/src/shared/Threading
    ${MYSQL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

link_directories(
    ${Boost_LIBRARY_DIRS}
)

add_executable(${EXECUTABLE_NAME}
    ${current_SRCS}
)

target_link_libraries(${EXECUTABLE_NAME}
    shared
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${MYSQL_LIBRARIES}
)

install_targets(/bin ${EXECUTABLE_NAME})
install_files(/etc ${EXECUTABLE_NAME}.conf.dist)
