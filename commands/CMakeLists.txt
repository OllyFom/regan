cmake_minimum_required(VERSION 3.15)

project(pjct_commands)

find_package(SQLiteCpp CONFIG REQUIRED)

set(COMMANDS_TARGET_NAME ${PROJECT_NAME})

set(COMMANDS_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(COMMANDS_HEADERS
    ${COMMANDS_ROOT_DIR}/cmds/monitor.hpp
    ${COMMANDS_ROOT_DIR}/cmds/analyser.hpp
    ${COMMANDS_ROOT_DIR}/commands_base.hpp
)

set(COMMANDS_SOURCES
    ${COMMANDS_ROOT_DIR}/cmds/analyser.cpp
)

add_library(${COMMANDS_TARGET_NAME} STATIC ${COMMANDS_HEADERS} ${COMMANDS_SOURCES})

target_include_directories(${COMMANDS_TARGET_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${COMMANDS_ROOT_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${COMMANDS_TARGET_NAME}
    PUBLIC
        storage
    PRIVATE 
        Boost::program_options
)