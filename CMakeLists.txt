cmake_minimum_required(VERSION 3.14..3.28)
project(try_googletest
        VERSION 0.0.1
        DESCRIPTION "Unit Tests"
        LANGUAGES CXX)

include(cmake/UpdateSubModules.cmake)
include(CTest)

file(GLOB_RECURSE WEBREQ_SOURCES
        "src/*.cpp"
)
list(REMOVE_ITEM WEBREQ_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_library(webreq_lib ${WEBREQ_SOURCES})

target_include_directories(webreq_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/connection
        ${CMAKE_CURRENT_SOURCE_DIR}/src/constant
        ${CMAKE_CURRENT_SOURCE_DIR}/src/file
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parser
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/configuration
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/mime
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/request
        ${CMAKE_CURRENT_SOURCE_DIR}/src/response
        ${CMAKE_CURRENT_SOURCE_DIR}/src/service
        ${CMAKE_CURRENT_SOURCE_DIR}/src/service/logger
        ${CMAKE_CURRENT_SOURCE_DIR}/src/util
        ${CMAKE_CURRENT_SOURCE_DIR}/src/util/color
)

add_subdirectory(tests)
add_subdirectory(external)
