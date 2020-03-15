	section .text,code

_memcpyw::
memcpyw::

	tst.w	d0
	beq.s	.Done

.loop:
	move.w	(a0)+,(a1)+
	subq.w	#1,d0
	bne.s 	.loop

.Done:
	rts
