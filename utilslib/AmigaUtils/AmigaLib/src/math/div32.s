
	section .text,code

; #####################################################
; Divide signed 32 bit value by signed 32 bit value.
;
; INPUT:
;	d0: Dividend
;	d1: divisor
;
; RETURN:
; 	d0: Result
;	d1: modulo
;
; TRASHED: --
;
; #####################################################

_div32s::
div32s::
	tst.l	d0
	bpl.s	.numpos
	neg.l	d0
	tst.l	d1
	bpl.s	.denompos
	neg.l	d1
	bsr.s	div32u
	neg.l	d1
	rts

.denompos:
	bsr.s	div32u
	neg.l	d0
	neg.l	d1
	rts

.numpos:
	tst.l	d1
	bpl.s	div32u
	neg.l	d1
	bsr.s	div32u
	neg.l	d0
	rts

;---------------------------------------------------------------------------

; #####################################################
; Divide unsigned 32 bit value by unsigned 32 bit value.
;
; INPUT:
;	d0: Dividend
;	d1: divisor
;
; RETURN:
; 	d0: Result
;	d1: modulo
;
; TRASHED: --
;
; #####################################################

_div32u::
div32u::

	move.l	d1,-(sp)
	tst.w	(sp)+		; can we do this easily? (is number < 65536)
	bne.s	.bigdenom	; no, we have to work for it
	swap.w	d0
	move.w	d0,d1
	beq.s	.smallnum
	divu.w	(sp),d1
	move.w	d1,d0

.smallnum:
	swap.w	d0
	move.w	d0,d1
	divu.w	(sp)+,d1
	move.w	d1,d0
	clr.w	d1
	swap	d1
	rts

.bigdenom:
	move.w	d2,(sp)
	move.l	d3,-(sp)
	moveq	#15,d3		; 16 times through the loop
	move.w	d3,d2
	exg 	d3,d1		; d3 is set
	swap	d0  		; $56781234
	move.w	d0,d1		; $00001234
	clr.w	d0		    ; $56780000

.dmls:
	add.l	d0,d0
	addx.l	d1,d1
	cmp.l	d1,d3
	bhi.s	.dmle
	sub.l	d3,d1
	addq.w	#1,d0

.dmle:
	dbf     d2,.dmls

	move.l	(sp)+,d3
	move.w	(sp)+,d2

	rts 
