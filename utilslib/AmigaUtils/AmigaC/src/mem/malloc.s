	section .text,code

	include lvo/exec_lib.i
	include exec/memory.i

	include utils/system/libraries.i

_malloc::
	move.l	4(sp),d0

malloc::
	addq	#4,d0

	movem.l	d0/a6,-(sp)
	move.l ExecBase,a6
	moveq	#MEMF_ANY,d1
	CALLLIB	AllocMem

	tst.l	d0
	beq.s	.Done

	move.l	d0,a0
	move.l	(sp),(a0)	; Remember Length of the allocated block
	addq	#4,d0

.Done:
	movem.l	(sp)+,d1/a6
	rts

_free::
	move.l	4(sp),d0

free::
	tst.l	d0
	beq.s	.Done

	subq	#4,d0
	move.l	d0,a1
	move.l	(a1),d0

	movem.l	a6,-(sp)
	move.l ExecBase,a6
	CALLLIB	FreeMem
	movem.l	(sp)+,a6

.Done:
	rts
