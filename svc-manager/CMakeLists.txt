cmake_minimum_required(VERSION 3.15)

set(SVC_MANAGER_TARGET_NAME svc-manager)
project(SVC_MANAGER_TARGET_NAME)

set(SVC_MANAGER_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(SVC_MANAGER_HEADERS
    ${SVC_MANAGER_ROOT_DIR}/svc-manager.hpp
)

set(SVC_MANAGER_SOURCES
    ${SVC_MANAGER_ROOT_DIR}/svc-manager.cpp
)

add_library(${SVC_MANAGER_TARGET_NAME} STATIC ${SVC_MANAGER_HEADERS} ${SVC_MANAGER_SOURCES})

target_include_directories(${SVC_MANAGER_TARGET_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${SVC_MANAGER_ROOT_DIR}>
    $<INSTALL_INTERFACE:include>
)

#target_link_libraries()