cmake_minimum_required(VERSION 3.15)

project(regan CXX)
set(REGAN_TARGET_NAME ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost CONFIG REQUIRED COMPONENTS program_options)

set(ACCESSORY_DIR ${CMAKE_SOURCE_DIR}/accessory)
set(COMMANDS_DIR ${CMAKE_SOURCE_DIR}/commands)

set(HEADERS
    ${COMMANDS_DIR}/cmds/analyser.hpp
    ${COMMANDS_DIR}/cmds/monitor.hpp
)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/main-cli.cpp
    ${ACCESSORY_DIR}/commands_fabric.cpp
    ${COMMANDS_DIR}/cmds/analyser.cpp
)

add_executable(${REGAN_TARGET_NAME} ${SOURCES})


target_include_directories(${REGAN_TARGET_NAME} PRIVATE
    ${ACCESSORY_DIR}
    ${COMMANDS_DIR}
    ${COMMANDS_DIR}/cmds
    ${CMAKE_SOURCE_DIR}
)

add_subdirectory(storage)
add_subdirectory(svc-manager)
add_subdirectory(svc-logger)
add_subdirectory(drv-comm-svc)
add_subdirectory(commands)

target_link_libraries(${REGAN_TARGET_NAME} 
    PRIVATE 
        Boost::program_options
    PUBLIC
        pjct_commands
        # TODO: unlink svc-manager, must be called by pjct_commands
        svc-manager
)
