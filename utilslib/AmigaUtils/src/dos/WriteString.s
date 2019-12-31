
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
; #####################################################

puts::
	move.l	stdout,d0

putsf::
	move.l	d0,d1			; FileHandle
	move.l	a0,d2			; Buffer
	jsr		strlen
	move.l	d0,d3			; Length

	move.l	a6,-(a7)
	move.l	DOSBase,a6
	CALLLIB	Write
	move.l	(a7)+,a6
	
	rts
