#ifndef _UTILSLIB_STRING_H
#define _UTILSLIB_STRING_H

#include <cstdint>
#include <cstdarg>

#include "utils/utils_def.h"

/**
 ************************************************************
 * Print formatted string (similar to the snprintf).
 * If the buffer is to small, the buffer is not exceeded
 * and the number of required characteres are still
 * returned (without the 0-byte).
 * Even if the buffer is to small, the resulting string
 * is guranteed to be 0-terminated.
 *
 * INPUT:  
 *	len:	Length of outputbuffer
 *	buffer: Outputbuffer
 *	format: Formatstring
 *	args:	Variable args for the format parameters
 *
 * Format arguments are pushed on the stack. All arguments
 * are assumed 32 bit on stack.
 *
 *	Format:
 *		%(-N*l)b - binary value
 *		%c - single 8 bit character
 *		%(-N*l)d - signed decimal integer
 *		%(-N*)s - string. If the pointer is null "[null]" is inserted.
 *		%(-N*l)u - unsigned 32 bit integer
 *		%(-N*l)X - 16 bit hex value uppercase
 *		%p - pointer
 *		%n - store the number of characters already written (pointer to uint32_t).
 *		%% - '%' character
 *
 *		Floating point formatters are not implemented and will always
 *		print 'INF'. They are only placeholders for compatibillity with
 *		standard printf familly.
 *
 *		%f - Floatingpoint.
 *		%e - Floatingpoint.
 *
 *		\n - $0d	CR
 *		\r - $0a	LF
 *		\t - $08	TAB
 *		\\ - '\'
 *
 *
 * Format modifiers:
 *  *		- The width of the a field is determined by a 32 bit parameter.
 * <N>	- A number that specifies the minimum width of the field.
 *		  If the number starts with '0' leading zeroes are padded.
 * -	- Field is right aligned, defsault is left alignment
 * l	- Argument is long (32 bit value), default is 16.
 *
 * RETURN: Length of the output string.
 *
 ************************************************************
 */
C_FUNCTION uint32_t vFormatString(char *buffer, uint32_t bufferLen, const char *format, va_list args);
C_FUNCTION uint32_t formatString(char *buffer, uint32_t bufferLen, const char *format, ...);

#endif // _UTILSLIB_STRING_H
