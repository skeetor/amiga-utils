
	include lvo/dos_lib.i
	include lvo/exec_lib.i

	include dos/dos.i

	include utils/utils_def.i
	include utils/system/libraries.i

	section .text,code

; #####################################################
; Load file from disk. Memory is allocated for the file
; and the user is responsible to free this memory with
; FreeMem.
;
; INPUT:
;	A0: Filename
;	D0:	Memory attributes as used with AllocMem
;
; RETURN:
;	D0:	Filesize read.
;	A0: Pointer to memory block or nullptr
;
;	If an error occurs A0 wil always be NULL and D0
;	contains a statuscode.
;
;	0 = Parameter error
;	1 = Memory allocation
;	2 = File size error
;	3 = File open error
;	4 = File read error
;
; In case of an error the memory is not allocated.
;
; Modified: D0,D1,A0,A1
;
; #####################################################

_LoadFile::

	movem.l	a1,-(sp)
	jsr		LoadFile
	movem.l	(sp)+,d1

	tst.l	d1
	beq.s	.Done

	move.l	d1,a1
	move.l	d0,(a1)

.Done:
	move.l	a0,d0
	rts

LoadFile::

	move.l	a0,d1
	bne.s	.GetSize

	moveq	#0,d0			; Parameter error
	move.l	d0,a0
	rts

.GetSize:
	movem.l	d2-d6/a6,-(sp)

	move.l	d0,d2			; Remember allocation flags
	move.l	a0,d4			; Remember filename

	jsr		GetFileSize

	tst.l	d0
	beq.s	.FilesizeWrong

	moveq	#-1,d1
	cmp.l	d1,d0
	bne.s	.TryAlloc

.FilesizeWrong:
	moveq	#2,d5
	moveq	#0,d6
	bra.s	.Done

.TryAlloc:
	move.l	d2,d1
	move.l	d0,d5			; Filesize
	move.l	ExecBase,a6
	CALLLIB	AllocMem
	move.l	d0,d6			; MemoryPtr
	bne.s	.OpenFile

	moveq	#1,d5
	moveq	#0,d6
	bra.s	.Done

.OpenFile:
	move.l	d4,d1
	move.l	#MODE_OLDFILE,d2
	move.l	DOSBase,a6
	CALLLIB	Open
	move.l	d0,d4			; Filehandle
	bne.s	.ReadFile

	bsr.s	.FreeMem
	moveq	#3,d5
	moveq	#0,d6
	bra.s	.Done

.ReadFile:
	move.l	d4,d1
	move.l	d6,d2
	move.l	d5,d3
	CALLLIB	Read
	cmp.l	d0,d5
	beq.s	.Close

	bsr.s	.FreeMem
	moveq	#4,d5
	moveq	#0,d6

.Close:
	move.l	d4,d1
	CALLLIB	Close

.Done:
	move.l	d5,d0
	move.l	d6,a0
	movem.l	(sp)+,d2-d6/a6
	rts

.FreeMem:
	move.l	d5,d0
	move.l	d6,a1
	move.l	ExecBase,a6
	JMPLIB	FreeMem
