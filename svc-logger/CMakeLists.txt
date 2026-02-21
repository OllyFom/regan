cmake_minimum_required(VERSION 3.15)

project(svc-logger)
set(SVC_LOGGER_TARGET_NAME ${PROJECT_NAME})

set(SVC_LOGGER_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(SVC_LOGGER_HEADERS
    ${SVC_LOGGER_ROOT_DIR}/logger/logger.h 
    ${SVC_LOGGER_ROOT_DIR}/logger/log-msg.h  
    ${SVC_LOGGER_ROOT_DIR}/net/IPAddress.h
    ${SVC_LOGGER_ROOT_DIR}/exceptions/LogException.h 
    ${SVC_LOGGER_ROOT_DIR}/utils/utils.h
    ${SVC_LOGGER_ROOT_DIR}/config.h 
)

set(SVC_LOGGER_SOURCES
    ${SVC_LOGGER_ROOT_DIR}/net/IPAddress.cpp 
    ${SVC_LOGGER_ROOT_DIR}/logger/log-msg.cpp   
    ${SVC_LOGGER_ROOT_DIR}/exceptions/LogException.cpp 
    ${SVC_LOGGER_ROOT_DIR}/utils/utils.cpp
)

add_library(${SVC_LOGGER_TARGET_NAME} STATIC ${SVC_LOGGER_HEADERS} ${SVC_LOGGER_SOURCES})

target_include_directories(${SVC_LOGGER_TARGET_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${SVC_LOGGER_ROOT_DIR}>
        $<INSTALL_INTERFACE:include>
)

#target_link_libraries()

