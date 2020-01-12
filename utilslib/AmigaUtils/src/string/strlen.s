; #####################################################
; Return the length of a C-String (0-terminated)
;
; INPUT:  A0: Address of string
; RETURN: D0: Length of string
;
; TRASHED: D0, A0
;
; #####################################################

	section .text,code

strlen::
    move.l  a0,d0

.loop:
	tst.b   (a0)+
	bne     .loop

    sub.l   a0,d0
    not.l   d0

    rts
