; #####################################################
; Returns the size of a file or -1 in case of an error.
; DOS library must already be opened.
;
; INPUT:
;	A0: Pointer to filename
;
; RETURN:
; D0.L: Length of file, -1 if an error occured.
;
; Modified: D0,D1,A0,A1
;
; #####################################################

	include lvo/dos_lib.i
	include dos/dos.i
	include utils/utils_def.i
	include utils/system/libraries.i

	section .text,code

_GetFileSize::
GetFileSize::
	; Create a BCPL aligned pointer
	movem.l	d2/a2-a4/a6,-(sp)
	move.l	DOSBase,a6

	STACK_ALLOC	fib_SIZEOF,4,d1
	move.l	d1,a4				; Original stackpointer
	move.l	a7,a2				; FileInfoBlock

	move.l	a0,d1
	moveq	#ACCESS_READ,d2
	CALLLIB	Lock
	tst.l	d0
	beq.s	.Error

	move.l	d0,a3				; Lock
	move.l	d0,d1
	move.l	a2,d2
	CALLLIB	Examine
	tst.l	d0
	bne.s	.Locked

.ErrorUnlock:
	moveq	#-1,d0
	move.l	d0,a2
	bra.s	.Unlock

.Locked:
	btst	#31,fib_DirEntryType(a2)
	beq.s	.ErrorUnlock		; Not a file

	move.l	fib_Size(a2),a2

.Unlock:
	move.l	a3,d1
	CALLLIB	UnLock
	move.l	a2,d0

.Done:
	move.l	a4,a7
	movem.l	(sp)+,d2/a2-a4/a6

	rts

.Error:
	moveq	#-1,d0
	bra.s	.Done

