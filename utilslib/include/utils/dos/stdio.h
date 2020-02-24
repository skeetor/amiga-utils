#ifndef _UTILSLIB_STDIO_I
#define _UTILSLIB_STDIO_I 1

#include <utils/utils_def.h>

C_FUNCTION void printString(const char *string REGISTER_PARAM(a0));
C_FUNCTION void writeString(uint32_t fileHandle REGISTER_PARAM(d0), const char *string REGISTER_PARAM(a0));

#endif // _UTILSLIB_STDIO_I
