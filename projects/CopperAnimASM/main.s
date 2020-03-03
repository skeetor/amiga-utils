	include exec/types.i
	include graphics/gfxbase.i

	include utils/hardware/custom_registers.i
	include utils/system/libraries.i
	include utils/gfx/gfxbase.i

	section .text,code

_start:

	moveq	#0,d0				; Version
	jsr		OpenDOSLibrary
	;moveq	#0,d0				; Version
	;jsr		OpenGraphicsLibrary

	;move.l	GraphicsBase,a6
	;move.l	gb_ActiView(a6),a0
	;move.l	v_ViewPort(a0),a0
	;move.l	vp_ColorMap(a0),a0
	;move.w	cm_Count(a0),d0
	;move.l	cm_ColorTable(a0),a1

	jsr	DebugInit

	lea		VerticalBlankIRQ,a0
	jsr		SetVBI

	jsr		SystemSave

	jsr		StartIntro
	beq		.Done

.MainLoop:
.WaitMouse:
	
	btst	#6,CIAAPRA				; Left button
	beq		.Stop

	jmp		.MainLoop

.Stop:
	jsr		StopIntro

.Done:
	sub.l	a0,a0
	jsr		SetVBI

	jsr		SystemRestore

.Quit:

	jsr		CloseDOSLibrary
	;jsr		CloseGraphicsLibrary

	moveq	#0,d0			;return 0 to the system
	rts

VerticalBlankIRQ:

	lea		VBICounter,a5
	add.l	#1,(a5)

	move.l	VBIProc,d0
	beq		.Done

	move.l	d0,a0
	jsr (a0)

.Done
	rts

DebugInit:
	lea		WaitMouseButtonTxt,a0
	jsr		printString

.WaitStart:
	
	jsr		CheckLeftMouseRelease
	beq		.WaitStart

	lea		NewlineTxt,a0
	jsr		printString

	lea		MouseButtonExitTxt,a0
	jsr		printString
	lea		NewlineTxt,a0
	jsr		printString

	rts

; ********************************
; ************* DATA *************
; ********************************

	section .data,data

	CNOP 0,2

VBICounter::	dc.l 0
WaitMouseButtonTxt: dc.b	'Press left mousebutton to start...',0
MouseButtonExitTxt: dc.b	'Press left mousebutton to exit application.',0
NewlineTxt: dc.b	$0d,$0a,0

	section .bss,bss

	CNOP 0,2

VBIProc::		ds.l 1
