cmake_minimum_required(VERSION 3.15)

project(regan CXX)
set(REGAN_TARGET_NAME ${PROJECT_NAME})

find_package(Boost CONFIG REQUIRED COMPONENTS program_options)
find_package(SQLiteCpp CONFIG REQUIRED)

set(ACCESSORY_DIR ${CMAKE_SOURCE_DIR}/accessory)
set(COMMANDS_DIR ${CMAKE_SOURCE_DIR}/commands)
set(STORAGE_DIR ${COMMANDS_DIR}/cmds/storage)

set(HEADERS
    ${COMMANDS_DIR}/cmds/analyser.hpp
    ${COMMANDS_DIR}/cmds/monitor.hpp
    ${STORAGE_DIR}/storage.hpp
)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/main-cli.cpp
    ${ACCESSORY_DIR}/commands_fabric.cpp
    ${COMMANDS_DIR}/cmds/analyser.cpp
    ${STORAGE_DIR}/storage.cpp
)

add_executable(${REGAN_TARGET_NAME} ${SOURCES})

target_link_libraries(${REGAN_TARGET_NAME} PRIVATE Boost::program_options SQLiteCpp)

target_include_directories(${REGAN_TARGET_NAME} PRIVATE
    ${ACCESSORY_DIR}
    ${COMMANDS_DIR}
    ${COMMANDS_DIR}/cmds
    ${CMAKE_SOURCE_DIR}
)

set_property(TARGET ${REGAN_TARGET_NAME} PROPERTY CXX_STANDARD 17)

add_subdirectory(srv-manager)