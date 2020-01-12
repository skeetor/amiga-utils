
	section .text,code

; #####################################################
; Multiply signed/unsigned 32 bit value by signed/unsigned32
; bit value.
;
; INPUT:
;	d0: Num1
;	d1: Num2
;
; RETURN:
; 	d0: Result
;
; TRASHED: d1
;
; #####################################################

mult32s::
mult32u::
	move.l	d2,-(sp)

	move.l	d0,-(sp)	; a
	mulu.w	d1,d0		; d0=al*bl
	move.l	d1,d2		; b
	mulu.w	(sp)+,d1	; d1=ah*bl
	swap	d2			; d2=bh
	mulu.w	(sp)+,d2	; d2=al*bh
	add.w	d2,d1
	swap	d1
	move.l	(sp)+,d2
	clr.w	d1
	add.l	d1,d0

	rts 
