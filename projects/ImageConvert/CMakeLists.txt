cmake_minimum_required(VERSION 3.0) 
project(ImageConvert)
set(TARGET ${PROJECT_NAME}) 

find_package(AmigaUtils CONFIG REQUIRED)
find_package(AmigaGCCUtils CONFIG REQUIRED)
include(AmigaGCCProject)

enable_language(AMIGA_VASM)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1")

set(SOURCES
	main.cpp
	ImageConvert.h
	ImageConvert.cpp
	PngFile.h
	PngFile.cpp
	gfxImage.h
	gfxImage.cpp
	AmigaFile.h
	AmigaFile.cpp
	Color32Info.h
	Color32Info.cpp
	ImageFile.h
	ImageFile.cpp
	PngFile.h
	PngFile.cpp
	QuantizerUtillities.h
	QuantizerUtillities.cpp
	PnnQuantizer.cpp
)

set(ASM_SOURCES
)

set_source_files_properties(${ASM_SOURCES} PROPERTIES LANGUAGE AMIGA_VASM)

add_executable(${TARGET} ${SOURCES} ${ASM_SOURCES})
target_link_libraries(${TARGET} AmigaUtils AmigaGCCUtils)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)  
