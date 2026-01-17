cmake_minimum_required(VERSION 3.15)
project(Regan)

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(SQLiteCpp REQUIRED)

set(ACCESSORY_DIR ${CMAKE_SOURCE_DIR}/accessory)
set(COMMANDS_DIR ${CMAKE_SOURCE_DIR}/commands)
set(STORAGE_DIR $${COMMANDS_DIR}/cmds/storage)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/main-cli.cpp
    ${ACCESSORY_DIR}/commands_fabric.cpp
    ${STORAGE_DIR}/storage.cpp
)

set(HEADERS
    ${COMMANDS_DIR}/cmds/analyzer.hpp
    ${COMMANDS_DIR}/cmds/monitor.hpp
    ${STORAGE_DIR}/storage.hpp
)

add_executable(regan ${SOURCES})

target_link_libraries(regan PRIVATE Boost::program_options SQLiteCpp)

target_include_directories(regan PRIVATE
    ${ACCESSORY_DIR}
    ${COMMANDS_DIR}
    ${COMMANDS_DIR}/cmds
    ${CMAKE_SOURCE_DIR}
)