	include exec/execbase.i
	include utils/hardware/custom_registers.i

	section .text,code

_WaitRaster::
WaitRaster::
	movem.l	d0,-(a7)

.loop:
	move.l	VPOSR,d0
	and.l	#$1ff00,d0

	tst.w	PAL_SYSTEM
	bne		.pal

	cmp.l	#256<<8,d0
	beq		.Done
	jmp		.loop

.pal:
	cmp.l	#303<<8,d0
	bne.b	.loop

.Done:
	movem.l	(a7)+,d0
	rts
