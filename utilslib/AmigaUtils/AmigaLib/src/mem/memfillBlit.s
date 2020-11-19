	section .text,code

; Fill the specified memory with a given byte. Semantics is the same
; as the C function memset but the name is different to avoid name
; clashes when using C libraries.
;
; INPUT:
;	d1.b: Fillbyte
;	d2.l: Length of memoryblock
;	d3.b: 1 = Wait for blitter to finish, 0 = return immediatly
;	a2: Pointer to memory
;
; RETURN:
;	---
;
; TRASHED: d0-d1/a0-a1

	include utils/hardware/custom_registers.i

_memfillBlit::
memfillBlit::
	move.l	d2,d0
	bne.s	.Start
	rts

.Start:
	move.l	a2,a0
	cmp.l	#16,d2
	bgt.s	.BlitFill
	subq	#1,d0

.ByteLoop:
	move.b	d1,(a0)+

.ByteFill:
	dbf		d0,.ByteLoop
	rts

.BlitFill:
	; Adjust the end of the buffer to an even address
	move.l	d0,a1
	add.l	a2,d0
	btst	#0,d0
	exg		d0,a1
	beq.s	.CheckOddStart

	move.b	d1,(a1)
	subq	#1,d0
	
.CheckOddStart:
	; Also adjust the begin of the buffer to an even address
	move.l	d0,a1
	move.l	a0,d0
	btst	#0,d0
	exg		d0,a1
	beq.s	.DoBlit

	move.b	d1,(a0)+
	subq	#1,d0

.DoBlit:
	move.w	d1,a1
	exg		d0,a1
	lsl.w	#8,d1
	move.b	d0,d1
	exg		d0,a1

	move.l	#$dff000,a1
	move.w	d1,BLTADAT(a1)			; Fillpattern
	move.l	#$01f00000,BLTCON0(a1)	; USE A and O
									; Minterm: A = 0
	move.l	#-1,BLTAFWM(a1)			; First/Last mask (take all bits)
	clr.l	BLTAMOD(a1)				; Modulo from source A
	lsr.l	#1,d0					; Number of words

	move.l a0,BLTDPTH(a1)
	move.w d0,BLTSIZE(a1)

	; Wait for blitter done?
	tst.b	d3
	beq.s	.Done

.BlitterWait:
	btst #14,DMACONR(a1)
	bne .BlitterWait

.Done:
	rts
