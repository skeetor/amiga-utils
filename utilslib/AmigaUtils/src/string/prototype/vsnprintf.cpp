
/**
 * This file is a working implementation of the sprintf style
 * string creation. It is not used though, and only serves as a prototype
 * for the ASM implementation.
 */

#include "utils/string/string.h"
#include "utils/string/numbers.h"

#define CONTROL_MODE	(1 << 0)
#define FORMAT_MODE		(1 << 1)
#define ZERO_FILL		(1 << 2)
#define ALIGN_RIGHT		(1 << 3)
#define PARAM_32		(1 << 4)
#define SIGNED			(1 << 5)

#define SAVE_CHAR(ch) \
if(p < e)\
{\
	*p++ = ch;\
}\
count++;

extern "C"
{

static uint32_t strlen(const char *src)
{
	const char *p = src;
	while(*p++);

	return p - src - 1;
}

static uint32_t insertString(char mode, uint32_t strLen, uint32_t fieldWidth, const char *source, char *&p, char *e)
{
	uint32_t count = 0;
	char c;
	char fillChar;
	uint32_t fillLen = 0;

	if (!(mode & ALIGN_RIGHT))
	{
		if (strLen == (uint32_t)-1)
		{
			// If we have to align right, we need to know the length
			strLen = strlen(source);
		}

		if (fieldWidth > strLen)
		{
			fillLen = fieldWidth - strLen;

			if (mode & ZERO_FILL)
				fillChar = '0';
			else
				fillChar = ' ';

			while (fillLen > 0)
			{
				SAVE_CHAR(fillChar);
				fillLen--;
			}
		}
	}

	const char *src = source;
	while ((c = *src++) != 0)
	{
		SAVE_CHAR(c);
	}

	strLen = src - source - 1;

	if (mode & ALIGN_RIGHT)
	{
		while (strLen < fieldWidth)
		{
			SAVE_CHAR(' ');
			strLen++;
		}
	}

	return count;
}

static uint32_t insertUnsignedNumber(uint32_t value, char mode, uint32_t base, uint32_t fieldWidth, char *&p, char *e)
{
	uint32_t count = 0;
	char valueStr[34];		// maximum numeric value can be a 32 bit binary string with +/
	uint32_t strLen;

	if (mode & PARAM_32)
		strLen = uLongToString(value, valueStr, sizeof(valueStr), base);
	else
		strLen = uWordToString(value, valueStr, sizeof(valueStr), base);

	valueStr[strLen] = 0;

	count += insertString(mode, strLen, fieldWidth, valueStr, p, e);

	return count;
}

static uint32_t insertSignedNumber(int32_t value, char mode, uint32_t base, uint32_t fieldWidth, char *&p, char *e)
{
	uint32_t count = 0;
	char valueStr[34];		// maximum numeric value can be a 32 bit binary string with +/
	uint32_t strLen;

	if (mode & PARAM_32)
		strLen = sLongToString(value, valueStr, sizeof(valueStr), base);
	else
		strLen = sWordToString(value, valueStr, sizeof(valueStr), base);

	valueStr[strLen] = 0;

	count += insertString(mode, strLen, fieldWidth, valueStr, p, e);

	return count;
}

}

C_FUNCTION uint32_t vsPrintString(char *buffer, uint32_t bufferLen, const char *format, va_list args)
{
	if (!buffer)
		bufferLen = 0;

	uint32_t count = 0;
	uint32_t fieldWidth = 0;
	char *p = buffer;
	char *e = &buffer[bufferLen];

	char c;
	char mode = 0;

	while ((c = *format++) != 0)
	{
		if (mode & CONTROL_MODE)
		{
			switch (c)
			{
				case 'n':
					SAVE_CHAR(0x0d);
				break;

				case 'r':
					SAVE_CHAR(0x0a);
				break;

				case 't':
					SAVE_CHAR(0x09);
				break;

				default:
					SAVE_CHAR(c);
				break;
			}

			mode = 0;
		}
		else if (mode & FORMAT_MODE)
		{
			switch (c)
			{
				case '*':
				{
					fieldWidth = va_arg(args, uint32_t);
				}
				continue;

				case 'c':
				{
					uint32_t ch = va_arg(args, uint32_t);

					SAVE_CHAR(ch & 0xff);
					mode = 0;
				}
				continue;

				case 'l':
					mode |= PARAM_32;
				continue;

				case '-':
					mode |= ALIGN_RIGHT;
				continue;

				case '0':
					mode |= ZERO_FILL;
				continue;

				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{					
					format--;
					fieldWidth = stringToUWord(format, 10000u, 10u, nullptr, (char **)&format);
				}
				continue;

				case 's':
				{
					const char *src = va_arg(args, const char *);
					if (!src)
						src = "[null]";

					count += insertString(mode, -1, fieldWidth, src, p, e);

					mode = 0;
				}
				continue;

				case 'p':
				{
					SAVE_CHAR('0');
					SAVE_CHAR('x');

					mode |= ZERO_FILL | PARAM_32 | ALIGN_RIGHT;
					fieldWidth = 8;
				}

				case 'x':
				case 'X':
				{
					uint32_t val = va_arg(args, uint32_t);

					count += insertUnsignedNumber(val, mode, 16u, fieldWidth, p, e);

					mode = 0;
				}
				continue;

				case 'b':
				{
					uint32_t val = va_arg(args, uint32_t);

					count += insertUnsignedNumber(val, mode, 2u, fieldWidth, p, e);

					mode = 0;
				}
				continue;

				case 'u':
				{
					uint32_t val = va_arg(args, uint32_t);

					count += insertUnsignedNumber(val, mode, 10u, fieldWidth, p, e);

					mode = 0;
				}
				continue;

				case 'd':
				{
					int32_t val = va_arg(args, int32_t);

					mode |= SIGNED;
					count += insertSignedNumber(val, mode, 10u, fieldWidth, p, e);

					mode = 0;
				}
				continue;

				case 'n':
				{
					uint32_t *p = va_arg(args, uint32_t *);
					if (p)
						*p = count;

					mode = 0;
				}
				continue;

				// floating point not supported yet.
				case 'e':
				case 'f':
				{
					SAVE_CHAR('I');
					SAVE_CHAR('N');
					SAVE_CHAR('F');
					mode = 0;
				}
				continue;

				case '%':
				default:
				{
					SAVE_CHAR(c);
					mode = 0;
				}
				continue;
			}
		}
		else
		{
			if (c == '%')
			{
				mode |= FORMAT_MODE;
				fieldWidth = 0;

				continue;
			}

			if (c == '\\')
			{
				mode |= CONTROL_MODE;
				continue;
			}

			SAVE_CHAR(c);
		}

	}

	SAVE_CHAR(0);
	count--;

	return count;
}
