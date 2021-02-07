
#include <cstdio>
#include <cstdlib>

#include <proto/exec.h>
#include <utils/time/time.h>

#include "utils/utils_def.h"

#include <utils/mem/memory.h>
#include <utils/dos/dos.h>
#include <utils/dos/stdio.h>

static const uint32_t mem_size = 16384;
static uint8_t *memBlockC;
static uint8_t *memBlock;

static bool runComparison(uint8_t val, uint32_t l, uint32_t offset, uint32_t loops, const char *tag)
{
	clock_t elapsedC, elapsed;

	Forbid();

	uint8_t cmp = val+2;

	memBlockC[offset-1] = cmp;
	memBlockC[offset+l+1] = cmp;

	memBlock[offset-1] = cmp;
	memBlock[offset+l+1] = cmp;

	elapsedC = clock();
	for(uint32_t i = 0; i < loops; i++)
		memset(memBlockC+offset, val, l);
	elapsedC = clock() - elapsedC;

	elapsed = clock();
	for(uint32_t i = 0; i < loops; i++)
		memfill(memBlock+offset, val, l);
	elapsed = clock() - elapsed;

	Permit();

	printf("    %s memset/memfill: %lu/%lu %.2f%%\r\n", tag, elapsedC, elapsed, (1.0f - ((float)elapsed / (float)elapsedC)) * 100.0);

	if (memcmp(memBlockC+offset, memBlock+offset, l) != 0)
		return false;

	if (memBlock[offset-1] != cmp)
	{
		printf("Buffer overflow at start\r\n");
		return false;
	}
	
	if (memBlock[offset+l+1] != cmp)
	{
		printf("Buffer overflow at end\r\n");
		return false;
	}

	return true;
}

STDARGS int main(int argc, char *argv[])
{
	uint32_t start = 0;
	uint32_t end = mem_size;
	uint32_t memFlags = MEMF_FAST;
	const char *memType = (memFlags == MEMF_FAST) ? "FAST" : "CHIP";

	if (argc >= 2)
		start = atoi(argv[1]);

	if (argc >= 3)
		end = atoi(argv[2]);

	uint32_t memSize = end + 4;

	if (start >= end)
	{
		printf("Invalid range\r\n");
		return 10;
	}

	if ((memBlockC = (uint8_t *)AllocMem(memSize, memFlags)) == nullptr)
	{
		printf("Failed to allocate %lu bytes of %s memory\r\n", memSize, memType);
		return 10;
	}

	if ((memBlock = (uint8_t *)AllocMem(memSize, memFlags)) == nullptr)
	{
		FreeMem(memBlockC, memSize);
		printf("Failed to allocate %lu bytes of %s memory\r\n", memSize, memType);
		return 10;
	}

	OpenDOSLibrary();

	printf("Testing memfill vs. memset %s mem - Range: %lu - %lu\r\n", memType, start, end);

	uint8_t val = 0;
	size_t loops = 10000;

	for (uint32_t l = start; l < end; l++)
	{
		printf("\r\nBlocksize: %lu/%lu\r\n", l, end);

		val++;

		// Even address
		if (!runComparison(val, l, 2, loops, "EVN"))
		{
			PrintFormatted("FAILED!!!\r\n");
			return 10;
		}

		// Odd address
		if (!runComparison(val, l, 1, loops, "ODD"))
		{
			PrintFormatted("FAILED!!!\r\n");
			return 10;
		}
	}

	CloseDOSLibrary();

	FreeMem(memBlockC, memSize);
	FreeMem(memBlock, memSize);

	return 0;
}
