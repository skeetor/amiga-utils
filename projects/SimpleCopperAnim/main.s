	include lvo/exec_lib.i
	
	include utils/hardware/custom_registers.i
	include utils/system/libraries.i
	include utils/graphics/gfxbase.i

COPPER_START_LINE	EQU		36
COPPER_PAL_END_LINE	EQU		313

	section .text,code

_start:

	moveq	#0,d0				; Version
	jsr		OpenDOSLibrary

	jsr		InitDebug

	lea		VerticalBlankIRQ,a0
	jsr		SetVBI

	move.w	#COPPER_START_LINE,ScrollLine

	jsr		SystemSave

	jsr		PrepareCopperlist
	beq		.Done

	jsr		RestartCopperlist

	lea		HWREGBASE,a5
	move.l	CopperListPtr,d0
	move.w	#$0080,DMACON_OFS(a5)
	move.l	d0,COP1LCH_OFS(a5)
	move.w	d0,COPJMP2_OFS(a5)
	move.w	#$8280,DMACON_OFS(a5)

	lea		CopperAnimation,a0
	move.l	a0,VBIProc

.MainLoop:
.WaitMouse:
	btst	#6,CIAAPRA				; Left button
	beq		.Stop

.WaitRelease:
	jmp		.MainLoop

.Stop:
	move.w	#$0080,DMACON_OFS(a5)
	jsr		ClearCopperlist

.Done:
	jsr		SystemRestore

.Quit:

	moveq	#0,d0			;return 0 to the system
	rts

PrepareCopperlist:

	move.l ExecBase,a6
	move.l #CopperListSize,d0
	moveq	#2,d1					; CHIP_MEM
	CALLLIB	AllocMem

.Done:
	move.l	d0,CopperListPtr
	rts

ClearCopperlist:

	move.l #CopperListSize,d0
	move.l	CopperListPtr,a1
	beq		.Done

	move.l ExecBase,a6
	CALLLIB	FreeMem
	
.Done:
	rts

VerticalBlankIRQ:

	move.l	VBIProc,d0
	beq		.Done

	move.l	d0,a0
	jsr (a0)

.Done
	rts

RestartCopperlist:

	move.l	CopperListPtr,a0
	move.l	#($0180<<16)+$0000,(a0)+
	move.l	#((COPPER_START_LINE<<8+$01)<<16)+$ff00,(a0)+
	move.l	#($0180<<16)+$0fff,(a0)+
	move.l	#$fffffffe,(a0)+

	rts

CopperAnimation:

	move.l	CopperListPtr,a2
	move.w	ScrollLine,d0
	add.w	#1,d0
	cmp.w	#$0100,d0
	blt.s	.Store

	; If we are on NTSC then we are done.
	tst.w	PAL_SYSTEM
	beq.s	.Restart

	cmp.w	#COPPER_PAL_END_LINE,d0
	bge.s	.Restart

	; On PAL we need to extend to copperlist
	; because we have to do an extra wait to
	; reach the area below NTSC lines.

	; Check if we already extended the copperlist
	lea		12.w(a2),a0
	cmp.w	#$ffe1,(a0)
	beq.s	.PALStore

	sub.l	#4,a0
	move.l	#$ffe1fffe,(a0)+		; wait for a column outside the visible area
	move.l	#$0000ff00,(a0)+		; the PAL lines are now addressed with 0-57
	move.l	#($0180<<16)+$0fff,(a0)+
	move.l	#$fffffffe,(a0)+

.PALStore
	lea		8.w(a2),a2
	bra.s	.Store

.Restart:
	jsr		RestartCopperlist

	move.l	#COPPER_START_LINE,d0
	
.Store:
	move.w	d0,ScrollLine
	lsl.w	#8,d0
	bset	#0,d0
	move.w	d0,4(a2)

.Done:
	rts

InitDebug:
	lea		WaitMouseButtonTxt,a0
	jsr		puts

.WaitStart:
	
	jsr		CheckLeftMouseRelease
	beq		.WaitStart

	lea		NewlineTxt,a0
	jsr		puts

	lea		MouseButtonExitTxt,a0
	jsr		puts
	lea		NewlineTxt,a0
	jsr		puts

	rts

; ********************************
; ************* DATA *************
; ********************************

	section .data,data

	CNOP 0,2

CopperList:
	dc.w $ffff, $fffe
	dc.w $ffff, $fffe
	dc.w $ffff, $fffe
	dc.w $ffff, $fffe
	dc.w $ffff, $fffe
	dc.w $ffff, $fffe
	dc.w $ffff, $fffe
	dc.w $ffff, $fffe
	dc.w $ffff, $fffe
	dc.w $ffff, $fffe
CopperEnd:	
	dc.w $ffff, $fffe		; WAIT END
CopperListSize = *-CopperList


WaitMouseButtonTxt: dc.b	'Press left mousebutton to start...',0
MouseButtonExitTxt: dc.b	'Press left mousebutton to exit application.',0
NewlineTxt: dc.b	$0d,$0a,0

	section .bss,bss

	CNOP 0,2

CopperListPtr:	ds.l 1
VBICounter:		ds.l 1
VBIProc:		ds.l 1
ScrollLine:		ds.w 1
