	include lvo/exec_lib.i

	include utils/macros/validate.i
	include utils/hardware/custom_registers.i
	include utils/system/libraries.i
	include utils/hardware/copper.i

COPPER_START_LINE			EQU		COPPER_TOP_LINE

MAX_COPPER_LINES			SET		20

; ********************************
; ************* CODE *************
; ********************************
	section .text,code

StartIntro::
	jsr		AllocCopperMem
	beq		.Quit

	jsr		InitCopperDown

	lea		HWREGBASE,a5
	move.l	CopperListPtr,d0
	move.w	#$0080,DMACON(a5)
	move.l	d0,COP1LCH(a5)
	move.w	d0,COPJMP2(a5)		; Trigger copper start, value doesn't matter
	move.w	#$8280,DMACON(a5)

	lea		CopperAnim,a2
	move.l	a2,VBIProc

.Quit:
	rts

StopIntro::
	lea		HWREGBASE,a5
	move.w	#$0080,DMACON(a5)
	jsr		FreeCopperMem

	rts

AllocCopperMem:

	move.l ExecBase,a6
	move.l #MAX_COPPER_LINES*4,d0
	moveq	#2,d1					; CHIP_MEM
	CALLLIB	AllocMem
	move.l	d0,a1

	move.l #$fffffffe,(a1)

.Done:
	move.l	d0,CopperListPtr
	rts

FreeCopperMem:

	move.l #MAX_COPPER_LINES*4,d0
	move.l	CopperListPtr,a1
	beq		.Done

	move.l ExecBase,a6
	CALLLIB	FreeMem
	
.Done:
	rts

InitCopperDown:

	move.w	#1,CopperInfoCount
	move.l	#WBScreen,CopperFinalPtr

	move.w	#COPPER_PAL_END_LINE,d1
	tst.w	PAL_SYSTEM
	bne.s	.PAL
	move.w	#COPPER_NTSC_END_LINE,d1

.PAL:
	lea		CopperInfoList,a0
	move.w	#3,CI_Direction.w(a0)		; Stepping speed
	move.w	#COPPER_START_LINE,d0
	move.w	d0,CI_CurLine.w(a0)
	move.w	d0,CI_FirstLine.w(a0)
	move.w	d1,CI_LastLine.w(a0)
	move.l	#Black,CI_Registers.w(a0)
	move.l	#FlagSwitchCallback,CI_DoneCallback.w(a0)

	rts

InitCopperUp:

	move.w	#3,CopperInfoCount
	move.l	#Color3,CopperFinalPtr

	move.w	#COPPER_PAL_END_LINE,d1
	move.w	#COPPER_TOP_LINE+90,d2
	move.w	#COPPER_TOP_LINE+180,d3
	tst.w	PAL_SYSTEM
	bne.s	.PAL
	move.w	#COPPER_NTSC_END_LINE,d1
	move.w	#COPPER_TOP_LINE+73,d2
	move.w	#COPPER_TOP_LINE+146,d3

.PAL:
	move.l	#StopFlagCallback,d4

	lea		CopperInfoList,a0
	move.w	#-1,CI_Direction.w(a0)
	move.w	d1,CI_CurLine.w(a0)
	move.w	#COPPER_TOP_LINE,CI_FirstLine.w(a0)
	move.w	d1,CI_LastLine.w(a0)
	move.l	#Black,CI_Registers.w(a0)
	move.l	d4,CI_DoneCallback.w(a0)

	lea		CopperInfo(a0),a0
	move.w	#-1,CI_Direction.w(a0)
	move.w	d1,CI_CurLine.w(a0)
	move.w	d2,CI_FirstLine.w(a0)
	move.w	d1,CI_LastLine.w(a0)
	move.l	#Color1,CI_Registers.w(a0)
	move.l	d4,CI_DoneCallback.w(a0)

	lea		CopperInfo(a0),a0
	move.w	#-1,CI_Direction.w(a0)
	move.w	d1,CI_CurLine.w(a0)
	move.w	d3,CI_FirstLine.w(a0)
	move.w	d1,CI_LastLine.w(a0)
	move.l	#Color2,CI_Registers.w(a0)
	move.l	d4,CI_DoneCallback.w(a0)

	rts

ReverseDirectionCallback:
	neg.w	CI_Direction.w(a0)
	rts

StopFlagCallback:
	move.w #0,CI_Direction.w(a0)		; Disable the current animation block
	;rts

	; Now check if we have stopped all blocks, ...
	moveq	#3,d5
	moveq	#0,d6

	lea		CopperInfoList,a0
	move.l	a0,a1

.Loop:
	cmp.w	#0,CI_Direction.w(a0)
	bne.s	.Next

	addq	#1,d6

.Next:
	add.l	#CopperInfo,a0
	subq	#1,d5
	bne.s	.Loop

	; ... if yes, we revert the animation.
	cmp.w	#3,d6
	bne.s	.Done

	neg.w	FlagDirection
	moveq	#3,d5
	move.l	a1,a0

.Loop1:
	move.w	FlagDirection,CI_Direction.w(a0)
	add.l	#CopperInfo,a0
	subq	#1,d5
	bne.s	.Loop1

	cmp.w	#-1,FlagDirection
	bne.s	.Done

	move.l	#DownScrollSwitch,VBIProc

.Done:
	rts

; We have to use an intermediate switch here
; because we should not switch in the middle of
; the callback. So we schedule the next VBI
; to setup the next part of the intro here.
;
; Actually we could change the VBI method, but
; shouldn't modify the copper info structures
; while in the callback.
FlagSwitchCallback:
	neg.w	CI_Direction.w(a0)

	move.l	#SwitchToUpscroll,VBIProc

	rts

DownScrollSwitch:
	move.l	#CopperAnim,VBIProc
	jsr		InitCopperDown
	jmp		CopperAnim

; Here we are in the VBI, but not in the
; callback, so it's safe to modify the info
; structures.
SwitchToUpscroll:
	; Next VBI can jump directly to the animation
	move.l	#CopperAnim,VBIProc
	jsr		InitCopperUp
	; Fall through to the animation so it is executed
	; without delay.

CopperAnim::

	lea		CopperInfoList,a3
	move.l	CopperListPtr,a4
	move.l	CopperFinalPtr,a5
	move.w	CopperInfoCount,d3
	jmp		CreateCopperList

; ********************************
; ************* DATA *************
; ********************************

	section .data,data

FlagDirection:		dc.w -1

Black:
	dc.w $0180, $0000
	dc.l 0

WBScreen:
	dc.w $0180, $005a
	dc.l 0

Color1:
	dc.w $0180, $0f00
	dc.l 0

Color2:
	dc.w $0180, $0fff
	dc.l 0

Color3:
	dc.w $0180, $0f00
	dc.l 0

	CNOP 0,2

	section .bss,bss

	CNOP 0,2

CopperListPtr:		ds.l 1
CopperFinalPtr:		ds.l 1
CopperInfoCount:	ds.w 1
CopperInfoList:		ds.b CopperInfo*4
