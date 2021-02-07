; #####################################################
; Print a debug message and waít for a mouseclick
;
; INPUT:  A0: Address of string
; RETURN: D0: Length of string
;
; TRASHED: D0, A0
;
; #####################################################

	section .text,code

DebugWait::
_DebugWait::
	jsr		PrintString

.WaitClick:
	jsr		CheckLeftMouseRelease
	beq		.WaitClick

	rts
