cmake_minimum_required(VERSION 3.15)
project(ImageConvert)
set(TARGET ${PROJECT_NAME})

set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/projects/ImageConvert)

set(SOURCES
	${PROJECT_SOURCE_DIR}/main.cpp

	${PROJECT_SOURCE_DIR}/AmigaFile.h
	${PROJECT_SOURCE_DIR}/AmigaFile.cpp
	${PROJECT_SOURCE_DIR}/ImageFile.h
	${PROJECT_SOURCE_DIR}/ImageFile.cpp
	${PROJECT_SOURCE_DIR}/PngFile.h
	${PROJECT_SOURCE_DIR}/PngFile.cpp

	${PROJECT_SOURCE_DIR}/Color32Info.h
	${PROJECT_SOURCE_DIR}/Color32Info.cpp
	${PROJECT_SOURCE_DIR}/gfxImage.h
	${PROJECT_SOURCE_DIR}/gfxImage.cpp
	${PROJECT_SOURCE_DIR}/ImageConvert.h
	${PROJECT_SOURCE_DIR}/ImageConvert.cpp

	${PROJECT_SOURCE_DIR}/PnnQuantizer.cpp
	${PROJECT_SOURCE_DIR}/QuantizerUtillities.h
	${PROJECT_SOURCE_DIR}/QuantizerUtillities.cpp

	${UTILSLIB_SOURCE_DIR}/AmigaUtils/AmigaLib/src/algorithms/bsearch.cpp
	ToolsLib.cpp
)

add_executable(ImageConvert ${SOURCES})

target_link_libraries(${TARGET}
	png
	zlib
)
