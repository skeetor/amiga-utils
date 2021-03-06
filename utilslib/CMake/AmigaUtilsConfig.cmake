set(AMIGA_UTILS_TARGET AmigaUtils)
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_LIST_DIR}") 

if (NOT AMIGA_PATH_PREFIX)
	include(GetAmigaPathPrefix)
endif()

get_filename_component(AMIGA_UTILS_PATH_PREFIX ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

find_library(AMIGA_UTILS_LIBRARY_PATH ${AMIGA_UTILS_TARGET} HINTS "${AMIGA_UTILS_PATH_PREFIX}/lib")
if (NOT AMIGA_UTILS_LIBRARY_PATH)
	message(FATAL_ERROR "${AMIGA_UTILS_LIBRARY_PATH} can not be determined!")
endif()

link_directories("${AMIGA_UTILS_PATH_PREFIX}/lib")

include_directories(${AMIGA_UTILS_PATH_PREFIX}/include) 
include_directories(${AMIGA_OS_PATH_PREFIX}/ndk13-include) 
include_directories(${AMIGA_OS_PATH_PREFIX}/ndk-include) 

set(AMIGA_UTILS_TARGET)
