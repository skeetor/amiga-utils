	IFND _UTILS_COPPER_I
_UTILS_COPPER_I EQU 1

COPPER_TOP_LINE				EQU		36
COPPER_NTSC_END_LINE		EQU		255
COPPER_PAL_END_LINE			EQU		308

; ********************************
; CopperInfo defines the state for
; a block wwhere a given set of copper settings
; should be used.
;
; CI_Direction:	Specify the direction as well as the stepping speed.
;				i.E. A value of 1/-1 will advance the current line
;				for one screenline. A higher value means that the speed
;				is faster (i.E. 5/-5). A value of 0 means that this block
;				is not moved.
; CI_CurLine:	Initially this is the starting line where the animation
;				should start and should be within the boundaries of
;				CI_First and CI_Last line. This will be updated with each
;				call to the animation according to CI_Direction.
; CI_FirstLine: Upper line of the block
; CI_LastLine:	Last line of the block
; CI_Registers: This points to a list of hardware register copper (MOVE) instructions
;				which will be copied to the copperlist. The list must be terminated
;				by a (32 bit) 0 value. This pointer doesn't need to be in Chipmem.
; CI_DoneCallback: Pointer to a callback which is called whenever the first or the
;				last line of the block has been reached. May be 0 if not needed.
;
; CopperInfo	size of the CopperInfo structure, to be used ie. for AllocMem or similar
;
	rsreset
CI_Direction:		rs.w 1
CI_CurLine:			rs.w 1
CI_FirstLine:		rs.w 1
CI_LastLine:		rs.w 1
CI_Registers:		rs.l 1
CI_DoneCallback:	rs.l 1
CopperInfo:			rs.b 0

	ENDIF
