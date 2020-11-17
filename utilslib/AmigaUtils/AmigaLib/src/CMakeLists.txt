
add_subdirectory(tools)

set(ASM_SOURCES
	${ASM_SOURCES}  
	
	${CMAKE_CURRENT_SOURCE_DIR}/SystemTakeover.s
	${CMAKE_CURRENT_SOURCE_DIR}/WaitRaster.s
	${CMAKE_CURRENT_SOURCE_DIR}/DebugPowerLED.s
	${CMAKE_CURRENT_SOURCE_DIR}/GFXLibrary.s
	${CMAKE_CURRENT_SOURCE_DIR}/MouseHelper.s

	${CMAKE_CURRENT_SOURCE_DIR}/dos/DOSLibrary.s
	${CMAKE_CURRENT_SOURCE_DIR}/dos/WriteString.s
	${CMAKE_CURRENT_SOURCE_DIR}/dos/GetFileSize.s
	${CMAKE_CURRENT_SOURCE_DIR}/dos/FileLoad.s
	${CMAKE_CURRENT_SOURCE_DIR}/dos/PrintF.s

	${CMAKE_CURRENT_SOURCE_DIR}/string/strlen.s
	${CMAKE_CURRENT_SOURCE_DIR}/string/to_string.s
	${CMAKE_CURRENT_SOURCE_DIR}/string/word_to_string.s
	${CMAKE_CURRENT_SOURCE_DIR}/string/string_to_word.s
	${CMAKE_CURRENT_SOURCE_DIR}/string/long_to_string.s
	${CMAKE_CURRENT_SOURCE_DIR}/string/string_to_long.s
	${CMAKE_CURRENT_SOURCE_DIR}/string/format_string.s

	${CMAKE_CURRENT_SOURCE_DIR}/mem/memcpyl.s
	${CMAKE_CURRENT_SOURCE_DIR}/mem/memcpyw.s
	${CMAKE_CURRENT_SOURCE_DIR}/mem/zcopyb.s
	${CMAKE_CURRENT_SOURCE_DIR}/mem/zcopyw.s
	${CMAKE_CURRENT_SOURCE_DIR}/mem/zcopyl.s

	${CMAKE_CURRENT_SOURCE_DIR}/copper/CreateCopperList.s
	${CMAKE_CURRENT_SOURCE_DIR}/copper/CreateCopperListC.s

	${CMAKE_CURRENT_SOURCE_DIR}/math/div32.s
	${CMAKE_CURRENT_SOURCE_DIR}/math/mult32.s
	${CMAKE_CURRENT_SOURCE_DIR}/math/rnd_xorshift.s

	${CMAKE_CURRENT_SOURCE_DIR}/sound/ptplayer/ptplayer.asm
)

set(SOURCES
	${SOURCES}

	${CMAKE_CURRENT_SOURCE_DIR}/algorithms/bsearch.cpp
)

set(ASM_SOURCES ${ASM_SOURCES} PARENT_SCOPE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
