
#include <cstdio>
#include <cstdlib>

#include <proto/exec.h>
#include <utils/time/time.h>

#include "utils/utils_def.h"
#include "utils/system/system.h"
#include "utils/debug/debug.h"

#include <utils/mem/memory.h>
#include <utils/dos/dos.h>
#include <utils/dos/stdio.h>
#include <utils/dos/options.h>

typedef void (*ErrorFunction)(void);

extern ErrorFunction ErrorFkt;
extern uint32_t VBICount;
extern uint32_t MaxFrames;

extern uint16_t BlitterValidation;
extern uint32_t BlitterFillCount;
extern uint32_t BlitterWaitCount;
extern uint32_t BlitterMemSize;
extern uint8_t *BlitterMem;

extern uint32_t CPUFillCount;
extern uint32_t CPUMemSize;
extern uint8_t *CPUMem;

extern "C" void BlitterFillTest(void);

#define OPTION(name, minp, maxp, mandatory, help)

const Option gOptions[] =
{
	 Option("--blitter", 1, 1, false, "<Memsize> Fill Memory with the blitter")
	,Option("--help", 0, 0, false)
	,Option("-h", 0, 0, false, "This help message")
	,Option("--cpu", 1, 1, false, "<Memsize> Fill memory with the CPU (default is FAST_MEM)")
	,Option("--frames", 0, 0, false, "Number of frames the tests should be performed. On PAL 50 (default) is one second.")
	,Option("--chipmem", 0, 0, false, "Use CHIP_MEM for the cpu test. (Only relevant if also --cpu is specified.")
	,Option("--validate", 0, 0, false, "After a blitter fill, the memory is validated if there are no buffer overruns or gaps.")
	,Option("--debugWait", 0, 0, false, "Wait for Mouseclick before start to attach a debugger")

	,Option()
};

static void usage(const Option *option)
{
	printf("USAGE: TestBlitterMemfill --blitter <Memsize> --cpu <Memsize> [--frames <FrameCount> (default is 50)] [--chipmem] [--validate] [--debugWait]\n");
	printf("Copyright by Gerhard W. Gruber in Vienna 2020\n");
	printf("Execute as many memfills as possible in the specified number of frames and print the results.\n");
	printf("Memfills is done either by CPU or the blitter. If both are given then they are done in parallel.\n");

	while(option->m_name)
	{
		printf("%s\n", option->m_name);

		if (option->m_help)
			printf("%s\n", option->m_help);

		option++;
	}
}

static void Cleanup(void)
{
	if (BlitterMem)
		FreeMem(BlitterMem, BlitterMemSize+8);

	if (CPUMem)
		FreeMem(CPUMem, CPUMemSize);

	BlitterMem = nullptr;
	CPUMem = nullptr;
}

static uint32_t getSizeValue(int argc, char **argv, const char *param)
{
	uint32_t sz = 0;
	int pos;

	// Ignore if the param is not found.
	if ((pos = FindParam(argc, argv, param, 1)) != -1)
	{
		if (!pos)
		{
			printf("%s requires a size value!\n", param);
			return -1;
		}

		sz = atoi(argv[pos]);
		if (!sz)
		{
			printf("Invalid value %s %s!\n", param, argv[pos]);
			return -1;
		}
	}

	return sz;
}

static bool validateCommandline(int argc, char **argv)
{
	const Option *option = nullptr;
	int rc = ValidateOptions(argc, argv, gOptions, &option);
	if (rc == -1)
		return true;

	if (argv[rc] == nullptr)
		printf("Missing mandatory parameter: %s\n", option->m_name);
	else
	{
		printf("Invalid argument %s for %s", argv[rc], option->m_name);
		if (option->m_help)
			printf(" %s", option->m_help);

		printf("\n");
	}

	return false;
}

STDARGS int main(int argc, char *argv[])
{
	if (argc <= 1)
	{
		usage(gOptions);
		return 0;
	}

	if (!validateCommandline(argc, argv))
		return 10;

	if (((FindParam(argc, argv, "--help") != -1) || FindParam(argc, argv, "-h") != -1))
	{
		usage(gOptions);
		return 0;
	}
	
	if (FindParam(argc, argv, "--validate") == -1)
		BlitterValidation = 0;
	else
		BlitterValidation = 1;

	uint32_t CpuMemType = MEMF_FAST;
	if (FindParam(argc, argv, "--chipmem") != -1)
		CpuMemType = MEMF_CHIP;

	if ((BlitterMemSize = getSizeValue(argc, argv, "--blitter")) == (uint32_t)-1)
		return 10;

	if ((CPUMemSize = getSizeValue(argc, argv, "--cpu")) == (uint32_t)-1)
		return 10;

	if ((MaxFrames = getSizeValue(argc, argv, "--frames")) <= 0)
		MaxFrames = 50;

	if (BlitterMemSize)
	{
		if ((BlitterMem = (uint8_t *)AllocMem(BlitterMemSize+8, MEMF_CHIP)) == nullptr)
		{
			Cleanup();

			printf("Failed to allocate %lu bytes of chipmem memory\n", BlitterMemSize);
			return 10;
		}
	}

	if (CPUMemSize)
	{
		if ((CPUMem = (uint8_t *)AllocMem(CPUMemSize, CpuMemType)) == nullptr)
		{
			Cleanup();
			printf("Failed to allocate %lu bytes of %s memory\n", CPUMemSize, CpuMemType);
			return 10;
		}
	}

	if (!BlitterMemSize && !CPUMemSize)
	{
		printf("At least one of --blitter or --cpu must be provided.\n");
		return 10;
	}

	OpenDOSLibrary();

	if (FindParam(argc, argv, "--debugWait") != -1)
	{
		printf("Frames: %lu BlitterSize: %lu  CPUSize: %lu Validation: %s %s\n"
				, MaxFrames
				, BlitterMemSize, CPUMemSize
				, (BlitterValidation) ? "true" : "false"
				, (CpuMemType == MEMF_CHIP) ? "MEMF_CHIP" : "MEMF_FAST"
			);
		
		DebugWait("Press left mouse to continue!\n\r");
	}

	SystemSave();
	BlitterFillTest();
	SystemRestore();

	if (ErrorFkt)
		ErrorFkt();
	else
	{
		printf("Frames: %lu BlitterCount: %lu  BlitterWait: %lu CPUCount: %lu\n"
				, MaxFrames
				, BlitterFillCount, BlitterWaitCount
				, CPUFillCount
			);
	}

	CloseDOSLibrary();

	Cleanup();

	return 0;
}
