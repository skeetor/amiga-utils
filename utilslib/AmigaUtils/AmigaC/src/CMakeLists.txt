
set(ASM_SOURCES
	${ASM_SOURCES}  

	${CMAKE_CURRENT_SOURCE_DIR}/mem/memcpy.s
	${CMAKE_CURRENT_SOURCE_DIR}/mem/malloc.s
	${CMAKE_CURRENT_SOURCE_DIR}/mem/memset.s

	${CMAKE_CURRENT_SOURCE_DIR}/math/div32.s
	${CMAKE_CURRENT_SOURCE_DIR}/math/mult32.s
)

set(SOURCES
	${SOURCES}
)

set(ASM_SOURCES ${ASM_SOURCES} PARENT_SCOPE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
