
add_subdirectory(zlib)
add_subdirectory(libpng)

set(ASM_SOURCES ${ASM_SOURCES})
set(SOURCES ${SOURCES})

set(ASM_SOURCES ${ASM_SOURCES} PARENT_SCOPE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
