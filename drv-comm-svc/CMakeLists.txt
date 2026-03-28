cmake_minimum_required(VERSION 3.15)

project(drv-comm-svc)
set(SVC_TARGET_NAME ${PROJECT_NAME})

set(SVC_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(DRV_COMMUNICATION_DIR ${CMAKE_CURRENT_LIST_DIR}/drv-comm-session)

set(SVC_HEADERS
    ${SVC_ROOT_DIR}/service.hpp
    ${DRV_COMMUNICATION_DIR}/drv-comm-session.hpp
    ${SVC_ROOT_DIR}/accessory/reg_unit_snapshot.hpp
)

set(SVC_SOURCES
    ${SVC_ROOT_DIR}/service.cpp
    ${DRV_COMMUNICATION_DIR}/drv-comm-session.cpp
    ${SVC_ROOT_DIR}/accessory/reg_unit_snapshot.cpp
)

add_executable(${SVC_TARGET_NAME} ${SVC_SOURCES} ${SVC_HEADERS})

#target_include_directories()

target_link_libraries(${SVC_TARGET_NAME}
    PRIVATE
        svc-logger
        storage
)