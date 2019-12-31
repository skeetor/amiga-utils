
	include lvo/dos_lib.i
	include utils/system/libraries.i

	section .text,code
	
DOSInit:
	move.l	DOSBase,a6
	CALLLIB	Output
	move.l	d0,stdout

	rts

DOSShutdown:
	rts

	CreateSystemLibrary DOS


; *******************************************************************************
; *******************************************************************************
; DATA
; *******************************************************************************
; *******************************************************************************

	section .data,data

	CNOP 0,2

stdout::	dc.l 1
DOSName::	dc.b	'dos.library',0
