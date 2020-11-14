
set(ASM_SOURCES
	${ASM_SOURCES}
)

set(SOURCES
	${SOURCES}

	${CMAKE_SOURCE_DIR}/include/utils/tools/png/png.h
	${CMAKE_SOURCE_DIR}/include/utils/tools/png/pnglibconf.h
	${CMAKE_SOURCE_DIR}/include/utils/tools/png/pngconf.h
	${CMAKE_SOURCE_DIR}/include/utils/tools/png/pngdebug.h
	${CMAKE_SOURCE_DIR}/include/utils/tools/png/pnginfo.h
	${CMAKE_SOURCE_DIR}/include/utils/tools/png/pngpriv.h
	${CMAKE_SOURCE_DIR}/include/utils/tools/png/pngstruct.h

	${CMAKE_CURRENT_SOURCE_DIR}/AmigaPng.c
	${CMAKE_CURRENT_SOURCE_DIR}/png.c
	${CMAKE_CURRENT_SOURCE_DIR}/pngerror.c
	${CMAKE_CURRENT_SOURCE_DIR}/pngget.c
	${CMAKE_CURRENT_SOURCE_DIR}/pngmem.c
	${CMAKE_CURRENT_SOURCE_DIR}/pngpread.c
	${CMAKE_CURRENT_SOURCE_DIR}/pngread.c
	${CMAKE_CURRENT_SOURCE_DIR}/pngrio.c
	${CMAKE_CURRENT_SOURCE_DIR}/pngrtran.c
	${CMAKE_CURRENT_SOURCE_DIR}/pngrutil.c
	${CMAKE_CURRENT_SOURCE_DIR}/pngset.c
	#${CMAKE_CURRENT_SOURCE_DIR}/pngtest.c
	${CMAKE_CURRENT_SOURCE_DIR}/pngtrans.c
	${CMAKE_CURRENT_SOURCE_DIR}/pngwio.c
	${CMAKE_CURRENT_SOURCE_DIR}/pngwrite.c
	${CMAKE_CURRENT_SOURCE_DIR}/pngwtran.c
	${CMAKE_CURRENT_SOURCE_DIR}/pngwutil.c
)

set(ASM_SOURCES ${ASM_SOURCES} PARENT_SCOPE)
set(SOURCES ${SOURCES} PARENT_SCOPE)

message(STATUS "libPNG 1.6.37 added")
