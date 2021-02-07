	include exec/memory.i

	include utils/system/libraries.i
	include utils/hardware/custom_registers.i

	section .text,code

_BlitterFillTest::
	movem.l	d2-d7/a2-a6,-(sp)
	clr.l	_ErrorFkt

.Main:

	move.w	#DMA_SET|DMA_DMAEN|DMA_BLTEN,HWR_DMACON
	move.w	#0,HWR_COLOR00

	lea		VerticalBlankIRQ,a0
	jsr		SetVBI

	move.b	#0,d3			; Do not wait for blitter
	move.b	#$33,d4

	move.l	_BlitterMemSize,d6
	beq.s	.NoBlitter

	; Set guardposts for the blitter memory
	move.l	_BlitterMem,a2
	move.l	#$deadbeef,(a2)
	add.l	#4,a2
	move.l	a2,a3
	add.l	d6,a3
	move.l	#$deadbeef,(a3)

.NoBlitter:
	move.l	_CPUMemSize,d7
	move.l	_CPUMem,a3
	moveq	#1,d5
	clr.l	_VBICount

.MainLoop:
	tst.l	d6
	beq.s	.DoCpuFill

	move.l	d6,d2
	move.b	d4,d1
	jsr		memfillBlit
	add.l	d5,_BlitterFillCount

.DoCpuFill:
	; start a parallel fill by CPU
	tst.l	d7
	beq.s	.CheckBlitterWait

	exg		a2,a3
	move.l	d7,d2
	move.b	d4,d1
	jsr		memfill
	exg		a2,a3
	add.l	d5,_CPUFillCount

.CheckBlitterWait:
	; No Blittermem -> no validation required
	tst.l	d6
	beq.s	.LoopDone
	
.BlitterWait:
	add.l	#1,_BlitterWaitCount
	btst #14,HWR_DMACONR
	bne .BlitterWait

.CheckValidation:
	tst.w	_BlitterValidation
	beq.s	.LoopDone

	jsr		ValidateMemFill
	tst.l	d0
	beq.s	.Quit

.LoopDone:
	; Change fillbyte for each loop
	add.b	d5,d4
	move.l	_MaxFrames,d0
	move.l	_VBICount,d1
	cmp.l	d0,d1
	blt		.MainLoop

.Quit:
	sub.l	a0,a0
	jsr		SetVBI

.Done:

	movem.l	(sp)+,d2-d7/a2-a6
	rts

VerticalBlankIRQ:
	add.l	#1,_VBICount
	rts

ValidateMemFill:
	move.l	a2,a0
	move.l	d2,d0
	move.l	d4,d1

	move.l	a2,a3
	sub.l	#4,a3
	cmp.l	#$deadbeef,(a3)
	bne.s	.FailLower

	move.l	a2,a3
	add.l	d2,a3
	cmp.l	#$deadbeef,(a3)
	beq.s	.Validate

	lea		_PrintFailUpper,a0
	move.l	a0,_ErrorFkt
	moveq	#0,d0
	rts

.ValidateLoop:
	cmp.b	(a0)+,d1
	bne.s	.Fail

.Validate:
	dbf		d0,.ValidateLoop
	moveq	#1,d0
	rts

.Fail:
	lea		_PrintFailValidation,a0
	move.l	a0,_ErrorFkt
	moveq	#0,d0
	rts

.FailLower:
	lea		_PrintFailLower,a0
	move.l	a0,_ErrorFkt
	moveq	#0,d0
	rts
	
_PrintFailLower::
	lea		LowerString,a0
	jsr		PrintString
	lea		BoundaryValidationFailString,a0
	jsr		PrintString
	rts

_PrintFailUpper::
	lea		UpperString,a0
	jsr		PrintString
	lea		BoundaryValidationFailString,a0
	jsr		PrintString
	rts

_PrintFailValidation::
	lea		ValidationFailString,a0
	jsr		PrintString
	rts

; ********************************
; ************* DATA *************
; ********************************

	section .data,data

	CNOP 0,2

LowerString: dc.b 'Lower ',0
UpperString: dc.b 'Upper ',0
BoundaryValidationFailString: dc.b 'boundary check failed!\r\n',0
ValidationFailString: dc.b 'Fill validation failed!\r\n',0

	section .bss,bss

	CNOP 0,2

_ErrorFkt::			ds.l 1

_CPUFillCount::		ds.l 1
_CPUMemSize::		ds.l 1
_CPUMem::			ds.l 1

_BlitterWaitCount::	ds.l 1
_BlitterFillCount::	ds.l 1
_BlitterMemSize::	ds.l 1
_BlitterMem::		ds.l 1
_BlitterValidation::ds.w 1

_VBICount::			ds.l 1
_MaxFrames::		ds.l 1
