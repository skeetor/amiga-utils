#ifndef _UTILSLIB_STDIO_I
#define _UTILSLIB_STDIO_I 1

#include <cstdarg>

#include <utils/utils_def.h>

/**
 * Print a string to stdout/filehandle
 */
C_FUNCTION void printString(const char *string REGISTER_PARAM(a0));
C_FUNCTION void writeString(uint32_t fileHandle REGISTER_PARAM(d0), const char *string REGISTER_PARAM(a0));

/**
 * Print/Write a string in printf style to stdout/filehandle.
 * For a detailed description of the parameters and returns value
 * see utils/string/string.h
 *
 * The default length of the output string is assumed as 128 characters.
 * If the resulting size is longer than that, an additional buffer has to be
 * allocated which also results in much slower performance. To avoid this
 * you can use the ...FormatedN versions where you can specify the default
 * buffer length.
 * Note that this does not affect strings which are below the default length.
 *
 * The same applies to the ...FormattedN methods. If the defaultlength is
 * exceeded, then a bigger buffer will be used. The string will always be
 * correctly printed in any case, so this is only needed if you know that
 * you print a lot of strings longer then the default.
 *
 */
C_FUNCTION void vPrintFormatted(const char *format REGISTER_PARAM(a0), va_list args REGISTER_PARAM(d1));
C_FUNCTION void printFormatted(const char *format REGISTER_PARAM(a0), ...);

C_FUNCTION void vWriteFormatted(uint32_t fileHandle REGISTER_PARAM(d0), const char *format REGISTER_PARAM(a0), va_list args REGISTER_PARAM(d1));
C_FUNCTION void writeFormatted(uint32_t fileHandle REGISTER_PARAM(d0), const char *format REGISTER_PARAM(a0), ...);

C_FUNCTION void vPrintFormattedN(uint32_t defaultMaxLen REGISTER_PARAM(a1), const char *format REGISTER_PARAM(a0), va_list args REGISTER_PARAM(d1));
C_FUNCTION void printFormattedN(uint32_t defaultMaxLen REGISTER_PARAM(a1), const char *format REGISTER_PARAM(a0), ...);

C_FUNCTION void vWriteFormattedN(uint32_t defaultMaxLen REGISTER_PARAM(a1), uint32_t fileHandle REGISTER_PARAM(d0), const char *format REGISTER_PARAM(a0), va_list args REGISTER_PARAM(d1));
C_FUNCTION void writeFormattedN(uint32_t defaultMaxLen REGISTER_PARAM(a1), uint32_t fileHandle REGISTER_PARAM(d0), const char *format REGISTER_PARAM(a0), ...);

#endif // _UTILSLIB_STDIO_I
