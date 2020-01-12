cmake_minimum_required(VERSION 3.0) 
project(PrintFormattedCPP)
set(TARGET ${PROJECT_NAME}) 

find_package(AmigaUtils CONFIG REQUIRED)
find_package(AmigaGCCUtils CONFIG REQUIRED)
include(AmigaGCCProject)

enable_language(AMIGA_VASM)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1")

set(SOURCES
	main.cpp
)

add_executable(${TARGET} ${SOURCES}) 
target_link_libraries(${TARGET} AmigaUtils AmigaGCCUtils)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)  
