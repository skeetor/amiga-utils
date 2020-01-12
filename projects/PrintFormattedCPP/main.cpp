
#include <cstdio>

#include <proto/exec.h>
#include <utils/time/time.h>

#include "utils/utils_def.h"

#include "utils/string/string.h"
#include <utils/devices/mouse.h>

int main(int argc, char *argv[])
{
	char timestr[128];
	char buffer[128];
	uint32_t value;
	uint32_t len = 0;

	memset(buffer, 0, sizeof(buffer));

	value = 0x1239f;
	len = formatString(buffer, sizeof(buffer), "%s: %lu = %08lX\n\r", "Convert", value, value);
	uint32_t slen = snprintf(timestr, sizeof(timestr), "%s: %lu = %08lX\n\r", "Convert", value, value);

	if (len != slen)
		printf("Length missmatch!!!\nA: %lu != C: %lu\n", len, slen);

	if(strcmp(buffer, timestr) != 0)
		printf("Output missmatch!!!\nA:[%s]\nC:[%s]\n", buffer, timestr);

	clock_t elapsed;
	puts("Time AmigaLib sprintf comparison\nPress left mouse to skip...\n");
	elapsed = clock();
	for(uint16_t i = 0; i <= 10000; i++)
	{
		len = formatString(buffer, sizeof(buffer), "%s: %lu = %08lX\n\r", "Convert", value, value);
		if(CheckLeftMouseRelease())
			break;
	}

	elapsed = clock() - elapsed;
	formatString(buffer, sizeof(buffer), "Elapsed: %s\n", utils::clockToTimeString(elapsed, timestr, sizeof(timestr)));
	puts(buffer);

	printf("Time clib sprintf comparison\nPress left mouse to skip...\n");
	elapsed = clock();
	for(uint16_t i = 0; i <= 10000; i++)
	{
		len = snprintf(buffer, sizeof(buffer), "%s: %lu = %08lX\n\r", "Convert", value, value);
		if(CheckLeftMouseRelease())
			break;
	}
	elapsed = clock() - elapsed;
	formatString(buffer, sizeof(buffer), "Elapsed: %s\n", utils::clockToTimeString(elapsed, timestr, sizeof(timestr)));
	puts(buffer);

	return 0;
}
