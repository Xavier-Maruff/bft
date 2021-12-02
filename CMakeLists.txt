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

set(bft_src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(bft_src
    ${bft_src_dir}/main.cpp
    ${bft_src_dir}/parser.cpp
    ${bft_src_dir}/asc.cpp
    ${bft_src_dir}/log.cpp
    ${bft_src_dir}/cli.cpp
    ${bft_src_dir}/codegen/codegen.cpp
    ${bft_src_dir}/codegen/c.cpp
    ${bft_src_dir}/codegen/js.cpp
    ${bft_src_dir}/codegen/rust.cpp
    ${bft_src_dir}/codegen/ada.cpp
    ${bft_src_dir}/codegen/fortran.cpp
)

#include_directories(${CMAKE_BINARY_DIR}/src)

find_package(Boost COMPONENTS program_options REQUIRED)
#include_directories(${Boost_INCLUDE_DIR})
#link_libraries(${Boost_LIBRARIES})

add_executable(bft ${bft_src})
target_include_directories(bft PUBLIC ${bft_src_dir} ${Boost_INCLUDE_DIR})
target_link_libraries(bft ${Boost_LIBRARIES})