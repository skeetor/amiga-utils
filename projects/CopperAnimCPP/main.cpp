
#include <cstdio>

#include <proto/exec.h>
#include <hardware/custom.h>

#include <utils/system/system.h>
#include <utils/devices/mouse.h>
#include <utils/hardware/copper.h>
#include <utils/hardware/custom_registers.h>

#define COPPER_START_LINE	COPPER_TOP_LINE
#define MAX_COPPER_LINES	20

static void FlagSwitchCallback(CopperInfo *ci);
static void StopFlagCallback(CopperInfo *ci);

// Copper color moves
static uint32_t Black[] =
{
	0x01800000
	, 0
};

static uint32_t WBScreen[] =
{
	0x0180005a
	, 0
};

static uint32_t Color1[] =
{
	0x01800f00
	, 0
};

static uint32_t Color2[] =
{
	0x01800fff
	, 0
};

static uint32_t Color3[] =
{
	0x01800f00
	, 0
};

static uint16_t gCopperInfoCount;
static CopperInfo gCopperInfo[3];
static uint32_t *gCopperListPtr;
static uint32_t *gCopperFinalPtr;
static VBICallbackProc gVBIProc;
static uint16_t gFlagDirection = -1;

static void DebugWait(void)
{
	printf("Press left mousbutton to start...");
	fflush(stdout);

	while(!CheckLeftMouseRelease());

	printf("\nPress left mousbutton to exit...\n");
}

void ReverseDirectionCallback(CopperInfo *ci)
{
	ci->Direction *= (uint16_t)-1;
}

static void VerticalBlankInterruptCallback(void)
{
	if(gVBIProc)
		gVBIProc();
}

static void CopperAnim(void)
{
	CreateCopperList(gCopperInfoCount, &gCopperInfo[0], gCopperListPtr, gCopperFinalPtr);
}

void InitCopperDown(void)
{
	gCopperInfoCount = 1;
	gCopperFinalPtr = &WBScreen[0];

	CopperInfo *ci = &gCopperInfo[0];

	ci->Direction = 3;
	ci->CurLine = COPPER_START_LINE;
	ci->FirstLine = COPPER_START_LINE;
	if(PAL_SYSTEM)
		ci->LastLine = COPPER_PAL_END_LINE;
	else
		ci->LastLine = COPPER_NTSC_END_LINE;
	ci->Registers = &Black[0];
	//ci->DoneCallback = ReverseDirectionCallback;
	ci->DoneCallback = &FlagSwitchCallback;
}

void InitCopperUp(void)
{
	gCopperInfoCount = 3;
	gCopperFinalPtr = &Color3[0];

	uint16_t bottom;
	uint16_t middle;
	uint16_t low;

	CopperInfoCallbackProc proc = &StopFlagCallback;

	if(PAL_SYSTEM)
	{
		bottom = COPPER_PAL_END_LINE;
		middle = COPPER_START_LINE+90;
		low = COPPER_START_LINE+180;
	}
	else
	{
		bottom = COPPER_PAL_END_LINE;
		middle = COPPER_START_LINE+73;
		low = COPPER_START_LINE+146;
	}

	CopperInfo *ci = &gCopperInfo[0];

	ci->Direction = -1;
	ci->CurLine = bottom;
	ci->FirstLine = COPPER_TOP_LINE;
	ci->LastLine = bottom;
	ci->Registers = &Black[0];
	ci->DoneCallback = proc;

	ci++;
	ci->Direction = -1;
	ci->CurLine = bottom;
	ci->FirstLine = middle;
	ci->LastLine = bottom;
	ci->Registers = &Color1[0];
	ci->DoneCallback = proc;

	ci++;
	ci->Direction = -1;
	ci->CurLine = bottom;
	ci->FirstLine = low;
	ci->LastLine = bottom;
	ci->Registers = &Color2[0];
	ci->DoneCallback = proc;
}

static void DownScrollSwitch(void)
{
	gVBIProc = &CopperAnim;
	InitCopperDown();
	CopperAnim();
}

static void StopFlagCallback(CopperInfo *ci)
{
	ci->Direction = 0;

	uint16_t cnt = 3;
	uint16_t total = 0;

	ci = &gCopperInfo[0];
	for(uint16_t i = 0; i < cnt; i++)
	{
		if(ci->Direction == 0)
			total++;

		ci++;
	}

	if(total == cnt)
	{
		gFlagDirection *= (uint16_t)-1;
		ci = &gCopperInfo[0];
		for(uint16_t i = 0; i < cnt; i++)
		{
			ci->Direction = gFlagDirection;
			ci++;
		}

		gVBIProc = &DownScrollSwitch;
	}
}

/**
 * Here we are in the VBI, but not in the
 * callback, so it's safe to modify the info
 * structures.
 */
static void SwitchToUpscroll(void)
{
	gVBIProc = &CopperAnim;
	InitCopperUp();
	CopperAnim();
}

/**
 * We have to use an intermediate switch here
 * because we can not switch in the middle of
 * the callback. So we schedule the next VBI
 * to setup the next part of the intro here.
 *
 * Actually we could change the VBI method, but
 * shouldn't modify the copper info structures
 * while in the callback.
 */
static void FlagSwitchCallback(CopperInfo *ci)
{
	ci->Direction *= (uint16_t)-1;
	gVBIProc = &SwitchToUpscroll;
}

void startAnimation(void)
{
	*gCopperListPtr = 0xfffffffe;

	InitCopperDown();

	Custom *hw = (Custom *)HWREGBASE;

	hw->dmacon = 0x0080;
	hw->cop1lc = (ULONG)gCopperListPtr;
	hw->copjmp2 = 1; // Trigger copper start, value doesn't matter
	hw->dmacon = 0x8280;

	gVBIProc = &CopperAnim;
}

void stopAnimation(void)
{
	Custom *hw = (Custom *)HWREGBASE;

	hw->dmacon = 0x0080;
	gVBIProc = nullptr;
}

STDARGS int main(int argc, char *argv[])
{
	uint32_t sz = sizeof(uint32_t)*MAX_COPPER_LINES;
	gCopperListPtr = (uint32_t *)AllocMem(sz, MEMF_CHIP);
	if(!gCopperListPtr)
	{
		printf("[ERR] Unable to allocate %u bytes of CHIPMEM! Aborting!\n", sz);
		return 10;
	}

	DebugWait();

	SystemSave();

	SetVBI(VerticalBlankInterruptCallback);

	startAnimation();

	while(!CheckLeftMouseRelease());

	SetVBI(nullptr);

	stopAnimation();

	SystemRestore();

	FreeMem(gCopperListPtr, sz);

	return 0;
}
