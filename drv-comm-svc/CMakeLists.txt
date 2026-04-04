cmake_minimum_required(VERSION 3.15)

project(drv-comm-svc)
set(SVC_TARGET_NAME ${PROJECT_NAME})

set(SVC_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(SVC_HEADERS
    ${SVC_ROOT_DIR}/service.hpp
    ${SVC_ROOT_DIR}/registry-run-monitor/registry-run-mon.hpp
    ${SVC_ROOT_DIR}/driver-session/driver-session.hpp
)

set(SVC_SOURCES
    ${SVC_ROOT_DIR}/service.cpp
    ${SVC_ROOT_DIR}/registry-run-monitor/registry-run-mon.cpp
)

add_executable(${SVC_TARGET_NAME} ${SVC_SOURCES} ${SVC_HEADERS})

#target_include_directories()

target_link_libraries(${SVC_TARGET_NAME}
    PRIVATE
    svc-logger
)