#include "utils/utils_def.h"
#include "utils/string/numbers.h"
#include <utils/algorithms/bsearch.h>

void OpenDOSLibrary(void)
{
}

void CloseDOSLibrary(void)
{
}

static bool isPowerOfTwo(uint32_t num)
{
	return ((num != 0) && ((num & (num - 1)) == 0));
}

static uint32_t uLongFromString(const char *p, const char *e, uint32_t base, RESULT_STATE *state, char **endPointer, uint32_t cutoff, uint32_t limit)
{
	if (base < 2 || base > 62)
	{
		if (state)
			*state = RESULT_ERROR;

		if (endPointer)
			*endPointer = (char *)p;

		return -1;
	}

	uint32_t val = 0;

	while (p < e)
	{
		unsigned char c = *p++;
		bool add = true;

		if (c >= '0' && c <= '9')
			c -= '0';
		else if (c >= 'a' && c <= 'z')
			c = (c - 'a') + 10;
		else if (c >= 'A' && c <= 'Z')
			c = (c - 'A') + 36;
		else
			add = false;

		if (c >= base)
			add = false;

		if (val == cutoff && c > limit)
			add = false;

		if (!add)
		{
			if (state)
			{
				if (c == 0)		// End of string reached
					*state = RESULT_OK;
				else
					*state = RESULT_INVALID_CHAR;
			}

			if (endPointer)
				*endPointer = (char *)(p-1);

			return val;
		}

		val *= base;
		val += c;
	}

	if (state)
	{
		if (val == 0)		// End of string reached
			*state = RESULT_OK;
		else
			*state = RESULT_INVALID_CHAR;
	}

	if (endPointer)
		*endPointer = (char *)p;

	return val;
}

static int32_t sLongFromString(const char *p, const char *e, uint32_t base, RESULT_STATE *state, char **endPointer, uint32_t cutoff, uint32_t limit)
{
	if (p < e && *p == '-')
		p++;

	return (int32_t)uLongFromString(p, e, base, state, endPointer, cutoff, limit);
}

C_FUNCTION uint16_t stringToUWord(const char *buffer, uint32_t bufferLen, uint32_t base, RESULT_STATE *state, char **endPointer)
{
	uint16_t cutoff = 0xffff / base;
	uint16_t limit = 0xffff % base;

	limit = limit * !isPowerOfTwo(base);

	return (uint16_t)uLongFromString(buffer, &buffer[bufferLen], base, state, endPointer, cutoff, limit);
}

C_FUNCTION int16_t stringToSWord(const char *buffer, uint32_t bufferLen, uint32_t base, RESULT_STATE *state, char **endPointer)
{
	uint16_t cutoff = 0x8000 / base;
	uint16_t limit = 0x8000 % base;

	limit = limit * !isPowerOfTwo(base);

	return (int16_t)sLongFromString(buffer, &buffer[bufferLen], base, state, endPointer, cutoff, limit);
}

C_FUNCTION uint32_t stringToULong(const char *buffer, uint32_t bufferLen, uint32_t base, RESULT_STATE *state, char **endPointer)
{
	uint32_t cutoff = 0xffffffff / base;
	uint32_t limit = 0xffffffff % base;

	limit = limit * !isPowerOfTwo(base);

	return uLongFromString(buffer, &buffer[bufferLen], base, state, endPointer, cutoff, limit);
}

C_FUNCTION int32_t stringToSLong(const char *buffer, uint32_t bufferLen, uint32_t base, RESULT_STATE *state, char **endPointer)
{
	uint32_t cutoff = 0x80000000 / base;
	uint32_t limit = 0x80000000 % base;

	limit = limit * !isPowerOfTwo(base);

	return sLongFromString(buffer, &buffer[bufferLen], base, state, endPointer, cutoff, limit);
}
