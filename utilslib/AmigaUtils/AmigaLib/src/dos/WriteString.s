
	include lvo/dos_lib.i
	include utils/system/libraries.i

	section .text,code

; #####################################################
; Write string to file.
;
; INPUT:
;	D0: Filehandle
;	A0: Address of string
;
; RETURN: D0: Number of written bytes
;
; Modified: D0-D1,A0-A1
;
; #####################################################

_PrintString::
PrintString::
	move.l	stdout,d0

_WriteString::
WriteString::
	movem.l	d2-d3/a6,-(sp)
	move.l	d0,d1			; FileHandle
	move.l	a0,d2			; Buffer
	jsr		strlen
	move.l	d0,d3			; Length

	move.l	DOSBase,a6
	CALLLIB	Write
	movem.l	(sp)+,d2-d3/a6

	rts
