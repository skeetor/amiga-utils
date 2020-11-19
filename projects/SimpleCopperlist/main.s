	include lvo/exec_lib.i
	
	include utils/hardware/custom_registers.i
	include utils/system/libraries.i

	section .text,code

_start:
	jsr		SystemSave

	jsr		PrepareCopperlist
	beq		.Done

	lea		HWREGBASE,a5
	move.w	#$0080,DMACON(a5)
	move.l	d0,COP1LCH(a5)
	move.w	d0,COPJMP1(a5)
	move.w	#$8280,DMACON(a5)

.WaitMouse:
	btst	#6,CIAAPRA
	bne		.WaitMouse

	move.w	#$0080,DMACON(a5)
	jsr		ClearCopperlist

.Done:
	jsr		SystemRestore

.Quit:

	moveq	#0,d0			;return 0 to the system
	rts

PrepareCopperlist:

	move.l ExecBase,a6
	moveq	#CopperListSize,d0
	moveq	#2,d1					; CHIP_MEM
	CALLLIB	AllocMem
	move.l	d0,CopperListPtr
	beq		.Done

	move.l	d0,a1
	lea		CopperList,a0
	moveq	#CopperListSize>>2,d1

.loop:
	move.l	(a0)+,(a1)+
	subq	#1,d1
	bne		.loop

.Done:
	tst.l	d0
	rts

ClearCopperlist:

	moveq #CopperListSize,d0
	move.l	CopperListPtr,a1
	beq		.Done

	move.l ExecBase,a6
	CALLLIB	FreeMem
	
.Done:
	rts

; ********************************
; ************* DATA *************
; ********************************

	section .data,data

	CNOP 0,2

CopperList:
	dc.w $0180, $0000		; move COL00 -> Black
	dc.w $9501, $ff00		; WAIT 120

	dc.w $0180, $0f00		; Red
	dc.w $d701, $ff00		; WAIT 215

	dc.w $0180, $0fb0		; Yellow
	dc.w $ffff, $fffe		; WAIT END
	
CopperListSize = *-CopperList


	section .data,data

	CNOP 0,2

ExecName::	dc.b	'exec.library',0

	section .bss,bss

	CNOP 0,2

CopperListPtr:	ds.l 1
