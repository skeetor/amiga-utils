cmake_minimum_required(VERSION 3.0)
project(TestBlitterMemfill)
set(TARGET ${PROJECT_NAME}) 

find_package(AmigaUtils CONFIG REQUIRED)
find_package(AmigaGCCUtils CONFIG REQUIRED)
include(AmigaGCCProject)
enable_language(AMIGA_VASM)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(ASM_SOURCES
	BlitterFill.asm
)

set(SOURCES
	main.cpp
	${ASM_SOURCES}
)

set_source_files_properties(${ASM_SOURCES} PROPERTIES LANGUAGE AMIGA_VASM)

# Build as executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} AmigaUtils AmigaGCCUtils)
