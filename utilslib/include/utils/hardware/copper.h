
#ifndef _UTILS_COPPER_I
#define _UTILS_COPPER_I

#include "utils/utils_def.h"

#define COPPER_TOP_LINE			36
#define COPPER_NTSC_END_LINE	255
#define COPPER_PAL_END_LINE		308

struct CopperInfo;
typedef void (*CopperInfoCallbackProc)(CopperInfo *copperInfo);

/***************************************************
 * CopperInfo defines the state for
 * a block wwhere a given set of copper settings
 * should be used.
 *
 * CI_Direction:	Specify the direction as well as the stepping speed.
 *				i.E. A value of 1/-1 will advance the current line
 *				for one screenline. A higher value means that the speed
 *				is faster (i.E. 5/-5). A value of 0 means that this block
 *				is not moved.
 * CI_CurLine:	Initially this is the starting line where the animation
 *				should start and should be within the boundaries of
 *				CI_First and CI_Last line. This will be updated with each
 *				call to the animation according to CI_Direction.
 * CI_FirstLine: Upper line of the block
 * CI_LastLine:	Last line of the block
 * CI_Registers: This points to a list of hardware register copper (MOVE)instructions
 *				which will be copied to the copperlist. The list must be terminated
 *				by a (32 bit) 0 value. This pointer doesn't need to be in Chipmem.
 * CI_DoneCallback: Pointer to a callback which is called whenever the first or the
 *				last line of the block has been reached. May be NULL if not needed.
 */

#pragma pack(push, 1) 
struct CopperInfo
{
	uint16_t Direction;
	uint16_t CurLine;
	uint16_t FirstLine;
	uint16_t LastLine;
	uint32_t *Registers;
	CopperInfoCallbackProc DoneCallback;
};
#pragma pack(pop)

/**
 * @param copperInfoItems:	Number of items in the CopperInfo list (A3) 1..N. If the value is 0
 *							the behaviour is undefined.
 * @param copperInfoPtr:	Pointer to an array of CopperInfo structures
 * @param copperListMem:	Pointer to a chipmem memoryarea wich is sufficiently
 *							large to recceive all copperinstructions.
 * @param finalRegisters:	Pointer to the last list of hardware registers which is appended
 *							at the end of the generated list. This list is (32 bit) 0 terminated
 *							and must contain valid copper (MOVE) instructions.
 */
C_FUNCTION void CreateCopperList(uint16_t copperInfoItems, CopperInfo *copperInfoPtr, void *copperListMem, void *finalRegisters);

#endif
