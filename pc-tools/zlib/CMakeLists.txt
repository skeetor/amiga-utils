cmake_minimum_required(VERSION 3.15)
project(zlib)
set(TARGET ${PROJECT_NAME})

set(ZLIB_SOURCE_DIR ${CMAKE_SOURCE_DIR}/utilslib/AmigaUtils/AmigaLib/src/tools/zlib)

set(SOURCES
		${ZLIB_SOURCE_DIR}/gzguts.h
		${ZLIB_SOURCE_DIR}/adler32.c
		${ZLIB_SOURCE_DIR}/compress.c
		${ZLIB_SOURCE_DIR}/deflate.h
		${ZLIB_SOURCE_DIR}/deflate.c
		${ZLIB_SOURCE_DIR}/gzclose.c
		${ZLIB_SOURCE_DIR}/gzlib.c
		${ZLIB_SOURCE_DIR}/gzwrite.c
		${ZLIB_SOURCE_DIR}/inffast.c
		${ZLIB_SOURCE_DIR}/zutil.h
		${ZLIB_SOURCE_DIR}/zutil.c
		${ZLIB_SOURCE_DIR}/crc32.h
		${ZLIB_SOURCE_DIR}/crc32.c
		${ZLIB_SOURCE_DIR}/gzread.c
		${ZLIB_SOURCE_DIR}/infback.c
		${ZLIB_SOURCE_DIR}/inffast.h
		${ZLIB_SOURCE_DIR}/inflate.h
		${ZLIB_SOURCE_DIR}/inflate.c
		${ZLIB_SOURCE_DIR}/inffixed.h
		${ZLIB_SOURCE_DIR}/inftrees.h
		${ZLIB_SOURCE_DIR}/inftrees.c
		${ZLIB_SOURCE_DIR}/trees.h
		${ZLIB_SOURCE_DIR}/trees.c
		${ZLIB_SOURCE_DIR}/uncompr.c
)

add_library(${TARGET} ${SOURCES})

