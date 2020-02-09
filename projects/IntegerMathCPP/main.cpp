
#include <cstdio>

#include <proto/exec.h>
#include <utils/time/time.h>

#include "utils/utils_def.h"

#include "utils/string/numbers.h"
#include <utils/devices/mouse.h>

int main(int argc, char *argv[])
{
	char buffer[128];
	uint32_t value;
	uint32_t len = 0;

	memset(buffer, '0', sizeof(buffer));
	buffer[0] = '6';
	buffer[1] = '0';
	buffer[2] = 0;

	/*char *p = nullptr;
	RESULT_STATE st = RESULT_ERROR;
	uint32_t r32 = stringToULong(buffer, sizeof(buffer)-1, 10, &st, &p);
	printf("Value: %lu  State: %lu, %p %p\n", r32, st, p, buffer);*/

/*	value = 3866565;
	len = uLongToString(value, buffer, 100u, 2u);
	buffer[len] = 0;
	printf("DUWORD - Base 2: %lu, Len: %u - %%%s\n", value, len, buffer);
	len = uLongToString(value, nullptr, 0);
	printf("DUWORD - Base 10: Len: %u\n", len);

	value = 65535;
	value = 0;
	len = uWordToString(value, buffer, 100u, 2u);
	buffer[len] = 0;
	printf(" UWORD - Base 2: %lu, Len: %u - %%%s\n", value, len, buffer);
	len = uWordToString(value, nullptr, 0);
	printf(" UWORD - Base 10: Len: %u\n", len);
*/
	// Performance test to see how long the full set of
	// of conversion takes.
	// 13:14.360 / 13:14.220 / 12:35.200
	// Comparing the gcc implementation of itoa (which is not as safe)
	// takes about 41 minutes.
/*	clock_t elapsed;
	printf("Testing AmigaLib uwordToString(0 - 65535)\n");
	elapsed = clock();
	for(value = 0; value <= 65535; value++)
	{
		for(uint16_t i = 2; i <= 62; i++)
		{
			len = uWordToString(value, buffer, sizeof(buffer)-1, i);
			buffer[len] = 0;
			//printf("%lu Base %u : Result: %s : length: %lu\n", value, i, buffer, len);
		}
	}

	elapsed = clock() - elapsed;
	printf("AmigaLib: %s\n", utils::clockToTimeString(elapsed, buffer, sizeof(buffer)));
*/

	clock_t elapsed;
	printf("Validate AmigaLib string conversion DWORD (0 - 65535) Base 2-62\nPress left mouse to abort...\n");
	elapsed = clock();
	for(uint16_t i = 2; i <= 62; i++)
	{
		clock_t loop = clock();

		for(value = 0; value <= 65535; value++)
		{
			if(CheckLeftMouseRelease())
			{
				i = 62;
				break;
			}

			uint32_t v32 = 0x34000000 + value;

			len = uLongToString(v32, buffer, sizeof(buffer)-1, i);
			buffer[len] = 0;

			RESULT_STATE state;
			char *p;
			uint32_t r32 = stringToULong(buffer, sizeof(buffer)-1, i, &state, &p);

			if(r32 != v32 || state == RESULT_ERROR)
				printf("Value: %05lu %010lu/%010lu - Base %02u  State: %lu Len: %02lu Value: %s\n", value, v32, r32, i, state, len, buffer);

			//printf("%lu Base %u : Result: %s : length: %lu\n", value, i, buffer, len);
		}

		loop = clock() - loop;
		printf("Base: %2u  %s\n", i, utils::clockToTimeString(loop, buffer, sizeof(buffer)));
	}

	elapsed = clock() - elapsed;
	printf("AmigaLib: %s\n", utils::clockToTimeString(elapsed, buffer, sizeof(buffer)));

	printf("Validate AmigaLib string conversion WORD (0 - 65535) Base 2-62\nPress left mouse to abort...\n");
	elapsed = clock();
	for(uint16_t i = 2; i <= 62; i++)
	{
		clock_t loop = clock();

		for(value = 0; value <= 65535; value++)
		{
			if(CheckLeftMouseRelease())
			{
				i = 62;
				break;
			}

			uint16_t v16 = value;

			len = uWordToString(v16, buffer, sizeof(buffer)-1, i);
			buffer[len] = 0;

			RESULT_STATE state;
			char *p;
			uint16_t r16 = stringToUWord(buffer, sizeof(buffer)-1, i, &state, &p);

			if(r16 != v16 || state == RESULT_ERROR)
				printf("Value: %05u %05u/%05u - Base %02u  State: %lu Len: %02lu Value: %s\n", value, v16, r16, i, state, len, buffer);

			//printf("%lu Base %u : Result: %s : length: %lu\n", value, i, buffer, len);
		}

		loop = clock() - loop;
		printf("Base: %2u  %s\n", i, utils::clockToTimeString(loop, buffer, sizeof(buffer)));
	}

	elapsed = clock() - elapsed;
	printf("AmigaLib: %s\n", utils::clockToTimeString(elapsed, buffer, sizeof(buffer)));

	return 0;
}
