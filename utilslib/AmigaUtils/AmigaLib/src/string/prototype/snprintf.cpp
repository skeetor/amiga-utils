#include "utils/string/string.h"

C_FUNCTION uint32_t sPrintString(char *buffer, uint32_t bufferLen, const char *format, ...)
{
    va_list args;
    va_start(args, format);

	uint32_t count = vsPrintString(buffer, bufferLen, format, args);

	va_end(args);

	return count;
}
