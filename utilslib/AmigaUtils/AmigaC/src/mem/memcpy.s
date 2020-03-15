	section .text,code

	include lvo/exec_lib.i

	include utils/utils_def.i
	include utils/system/libraries.i

_memcpy::
	move.l	4(sp),a1
	move.l	8(sp),a0
	move.l	12(sp),d0

memcpy::

	movem.l	a6,-(sp)
	move.l	ExecBase,a6
	CALLLIB	CopyMem

	movem.l	(sp)+,a6
	move.l	12(sp),d0

	rts
