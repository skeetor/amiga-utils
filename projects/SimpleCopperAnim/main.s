	include lvo/exec_lib.i
	

	include utils/hardware/custom_registers.i
	include utils/system/libraries.i
	include utils/graphics/gfxbase.i

VBI_DELAY_COUNT		EQU		3
COPPER_START_LINE	EQU		36

COLOR_BAR_TOP		EQU		$00f
COLOR_BAR_MIDDLE	EQU		$fff
COLOR_BAR_BOTTOM	EQU		$f00

	section .text,code

_start:

	moveq	#0,d0				; Version
	jsr		OpenDOSLibrary

	jsr		Init

	lea		VerticalBlankIRQ, a0
	jsr		SetVBI

	move.w	#COPPER_START_LINE, ScrollLine

	jsr		PrepareCopperlist
	beq		.Quit

	jsr		SystemSave

	lea		HWREGBASE,a5
	
	move.l	CopperListPtr,d0
	move.w	#$0080, DMACON_OFS(a5)
	move.l	d0, COP1LCH_OFS(a5)
	move.w	d0,COPJMP2_OFS(a5)
	move.w	#$8280, DMACON_OFS(a5)

	lea		CopperAnimation, a2
	move.l	a2, VBIProc

.AnimLoop:
.WaitMouse:
	
	btst	#6, CIAAPRA				; Left button
	beq		.Stop

	;btst	#2, POTGOR				; Right button
	;bne		.AnimLoop

	;jsr		CopperAnimation

.WaitRelease:
	;btst	#2, POTGOR				; Right button
	;beq		.WaitRelease

	jmp		.AnimLoop

.Stop:
	move.w	#$0080, DMACON_OFS(a5)
	jsr		ClearCopperlist

.Done:
	jsr		SystemRestore

.Quit:

	moveq	#0,d0			;return 0 to the system
	rts

PrepareCopperlist:

	move.l ExecBase,a6
	moveq #CopperListSize,d0
	moveq	#2, d1					; CHIP_MEM
	CALLLIB	AllocMem
	move.l	d0,a1
	beq		.Done

	move.l	d0,a1
	lea		CopperList,a0
	move.w	#CopperListSize>>2,d1

.loop:
	move.l	(a0)+,(a1)+
	sub.w	#1,d1
	bne		.loop

.Done:
	move.l	d0,CopperListPtr
	rts

ClearCopperlist:

	moveq #CopperListSize,d0
	move.l	CopperListPtr,a1
	beq		.Done

	move.l ExecBase,a6
	CALLLIB	FreeMem
	
.Done:
	rts

VerticalBlankIRQ:

	add.l	#1, VBICounter

	move.l	VBIProc, d0
	beq		.Done

	move.l	d0,a0
	jsr (a0)

.Done
	rts

CopperAnimation:

	move.l	CopperListPtr, d0
	beq		.Done

	move.l	d0,a2
	move.w	ScrollLine, d0
	move.w	d0,d1	
	add.w	#$0001, d0
	cmp.w	d0,d1
	blt		.Store
	move.w	#COPPER_START_LINE, d0

.Store:
	move.w	d0, ScrollLine
	lsl.w	#8, d0
	bset	#0, d0
	move.w	d0, 4(a2)

.Done:
	rts


Init:
	lea		WaitMouseButtonTxt, a0
	jsr		puts

.WaitStart:
	
	jsr		WaitLeftMouseRelease
	beq		.WaitStart

	lea		NewlineTxt, a0
	jsr		puts

	lea		MouseButtonExitTxt, a0
	jsr		puts
	lea		NewlineTxt, a0
	jsr		puts

	rts

; ********************************
; ************* DATA *************
; ********************************

	section .data,data

	CNOP 0,2

CopperList:
	dc.w $0180, COLOR_BAR_TOP
	dc.w COPPER_START_LINE<<8+$01, $ff00
	
	dc.w $0180, COLOR_BAR_BOTTOM
	dc.w $ffff, $fffe		; WAIT END

	dc.w $0180, $0f00		; Red
	dc.w $d701, $ff00		; WAIT

	dc.w $0180, $0fb0		; Yellow
	dc.w $ffff, $fffe		; WAIT END

	dc.w $ffff, $fffe		; WAIT END
	dc.w $ffff, $fffe		; WAIT END
CopperEnd:	
	dc.w $ffff, $fffe		; WAIT END
	
CopperListSize = * - CopperList


WaitMouseButtonTxt: dc.b	'Press left mousebutton to start...',0
MouseButtonExitTxt: dc.b	'Press left mousebutton to exit application.',0
NewlineTxt: dc.b	$0d,$0a,0

	section .bss,bss

	CNOP 0,2

CopperListPtr:	ds.l 1
VBICounter:		ds.l 1
VBIProc:		ds.l 1
ScrollLine:		ds.w 1
