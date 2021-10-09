cmake_minimum_required(VERSION 3.10)
project(bft VERSION 1.0 DESCRIPTION "Multi-target optimizing brainf*ck transpiler")

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building with DEBUG_LOG expansion")
    add_compile_definitions(DEBUG)
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_BINARY_DIR}/src)

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_libraries(${Boost_LIBRARIES})

add_executable(bft
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/parser.cpp
    ${CMAKE_SOURCE_DIR}/src/asc.cpp
    ${CMAKE_SOURCE_DIR}/src/log.cpp
    ${CMAKE_SOURCE_DIR}/src/cli.cpp
    ${CMAKE_SOURCE_DIR}/src/codegen/codegen.cpp
    ${CMAKE_SOURCE_DIR}/src/codegen/c.cpp
    ${CMAKE_SOURCE_DIR}/src/codegen/js.cpp
    ${CMAKE_SOURCE_DIR}/src/codegen/rust.cpp
    ${CMAKE_SOURCE_DIR}/src/codegen/ada.cpp
    ${CMAKE_SOURCE_DIR}/src/codegen/fortran.cpp
)