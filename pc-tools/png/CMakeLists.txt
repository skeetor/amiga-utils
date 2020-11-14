cmake_minimum_required(VERSION 3.15)
project(png)
set(TARGET ${PROJECT_NAME})

set(PNGLIB_SOURCE_DIR ${CMAKE_SOURCE_DIR}/utilslib/AmigaUtils/AmigaLib/src/tools/libpng)

set(SOURCES
		${PNGLIB_SOURCE_DIR}/png.c
		${PNGLIB_SOURCE_DIR}/pngerror.c
		${PNGLIB_SOURCE_DIR}/pngget.c
		${PNGLIB_SOURCE_DIR}/pngmem.c
		${PNGLIB_SOURCE_DIR}/pngpread.c
		${PNGLIB_SOURCE_DIR}/pngread.c
		${PNGLIB_SOURCE_DIR}/pngrio.c
		${PNGLIB_SOURCE_DIR}/pngrtran.c
		${PNGLIB_SOURCE_DIR}/pngrutil.c
		${PNGLIB_SOURCE_DIR}/pngset.c
		${PNGLIB_SOURCE_DIR}/pngtest.c
		${PNGLIB_SOURCE_DIR}/pngtrans.c
		${PNGLIB_SOURCE_DIR}/pngwio.c
		${PNGLIB_SOURCE_DIR}/pngwrite.c
		${PNGLIB_SOURCE_DIR}/pngwtran.c
		${PNGLIB_SOURCE_DIR}/pngwutil.c

		${PNGLIB_SOURCE_DIR}/AmigaPng.c
)

add_library(${TARGET} ${SOURCES})

