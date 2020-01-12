	section .text,code

_memcpyl::
memcpyl::

	tst.l	d0
	beq.s	.Done

.loop:
	move.l	(a0)+,(a1)+
	subq.l	#1,d0
	bne.s	.loop

.Done:
	rts
