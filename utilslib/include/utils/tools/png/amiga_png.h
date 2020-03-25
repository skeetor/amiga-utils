#ifndef _AMIGA_PNG_H
#define _AMIGA_PNG_H

#include <utils/utils_def.h>

#define PNG_MAX_MAGIC_LEN	8

#ifdef AMIGA_OS
	#define PNG_CALLTYPE __stdargs
#else
	#define PNG_CALLTYPE
#endif

#define PNG_IMPEXP PNG_CALLTYPE 

#include "png.h"

/**
 * Workaround to use the setjmp function with the png
 * library.
 */
C_FUNCTION int PngSetJmp(void *ptr);

#endif // _AMIGA_PNG_H
