cmake_minimum_required(VERSION 3.15)

project(storage)
set(STORAGE_TARGET_NAME ${PROJECT_NAME})

find_package(SQLiteCpp CONFIG REQUIRED)

set(STORAGE_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(STORAGE_HEADERS
    ${STORAGE_ROOT_DIR}/storage.hpp
)

set(STORAGE_SOURCES
    ${STORAGE_ROOT_DIR}/storage.cpp
)

add_library(${STORAGE_TARGET_NAME} STATIC ${STORAGE_HEADERS} ${STORAGE_SOURCES})

target_include_directories(${STORAGE_TARGET_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${STORAGE_ROOT_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${STORAGE_TARGET_NAME} PUBLIC SQLiteCpp)