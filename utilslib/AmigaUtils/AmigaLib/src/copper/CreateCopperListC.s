; ********************************
; ************* CODE *************
; ********************************
	section .text,code

_CreateCopperList::
	movem.l	d2-d3/a2-a5,-(a7)

	move.w	d0,d3
	move.l	a0,a3
	move.l	a1,a4
	move.l	d1,a5

	jsr		CreateCopperList

	movem.l	(a7)+,d2-d3/a2-a5

	rts
