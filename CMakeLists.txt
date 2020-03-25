cmake_minimum_required(VERSION 3.15)
project(Amiga-Utils)
set(TARGET ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps -fverbose-asm")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -save-temps -fverbose-asm")

set(UTILSLIB_SOURCE_DIR ${CMAKE_SOURCE_DIR}/utilslib)
include_directories(${UTILSLIB_SOURCE_DIR}/include)
include_directories(${UTILSLIB_SOURCE_DIR}/include/utils/tools/zlib)
include_directories(${UTILSLIB_SOURCE_DIR}/include/utils/tools/png)

add_subdirectory(pc-tools)
