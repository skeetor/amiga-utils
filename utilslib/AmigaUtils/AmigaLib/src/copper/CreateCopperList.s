	include lvo/exec_lib.i

	include utils/macros/validate.i
	include utils/hardware/copper.i

; ********************************
; ************* CODE *************
; ********************************
	section .text,code


; ********************************
; Params:
; A3:	Pointer to an array of CopperInfo structures
; A4:	Pointer to a chipmem memoryarea wich is sufficiently
;		large to recceive all copperinstructions.
; A5:	Pointer to the last list of hardware registers which is appended
;		at the end of the generated list. This list is (32 bit) 0 terminated
;		and must contain valid copper (MOVE) instructions.
; D3:	Number of items in the CopperInfo list (A3) 1..N. If the value is 0
;		the behaviour is undefined.
;
;
; Used registers: D0-D3, A0-A4

CreateCopperList::

	moveq	#0,d2

.DoCopperLine:
	move.w	d2,d0
	lsl.w	#4,d0
	VALIDATE_EQ 16,CopperInfo		; Multiply. Fail if the CopperInfo struct size changes

	lea		0(a3,d0),a2

	move.l	CI_Registers.w(a2),d0
	beq.s	.DoCurLine

	move.l	d0,a0
	move.l	a4,a1
	jsr		zcopyl
	lea		-4(a1),a4

.DoCurLine:
	move.w	CI_CurLine.w(a2),d1
	move.w	CI_Direction.w(a2),d0
	beq.s	.StoreCurLine
	add.w	d0,d1

	btst	#15,d0
	bne.s	.ScrollUp
	
	move.w	CI_LastLine.w(a2),d0
	cmp.w	d0,d1
	ble.s	.StoreCurLine
	bra.s	.DoCallback

.ScrollUp:
	move.w	CI_FirstLine.w(a2),d0
	cmp.w	d0,d1
	bge.s	.StoreCurLine

.DoCallback:
	move.l	CI_DoneCallback.w(a2),d1
	beq		.NoCallback
	move.l	a2,a0				; So we are compatible with a gcc method call.
								; d1/a0/a1 can be scratched, d0 must be saved
								; Other registers should be saved by the
								; compiler anyway.
	move.l	d1,a1
	movem.w	d0,-(a7)
	jsr		(a1)
	movem.w	(a7)+,d0

.NoCallback:
	move.w	d0,d1

.StoreCurLine:
	move.w	d1,CI_CurLine.w(a2)

	cmp.w	#$0100,d1			; Below NTSC line?
	blt.s	.StoreDownCopper

	tst.w	PAL_SYSTEM
	beq.s	.Continue

	move.l	#$ffe1fffe,(a4)+
	sub.w	#$0100,d1

.StoreDownCopper:
	lsl.w	#8,d1
	bset	#0,d1
	swap	d1
	move.w	#$ff00,d1
	move.l	d1,(a4)+

.Continue:
	addq	#1,d2
	cmp.w	d2,d3
	bne		.DoCopperLine

	move.l	a5,a0
	move.l	a4,a1
	jsr		zcopyl
	lea		-4(a1),a4

	move.l	#$fffffffe,(a4)

	rts
