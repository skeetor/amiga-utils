; #####################################################
; Converts an unsigned 16 bit number to a decimal c-string.
;
; INPUT:
;	A0: Address of output buffer
;	D0.w: Number to convert
;	D1.w: size of outputbuffer
;
; RETURN: D0.L: Length of string
; If the provided memory was to short, the output memory
; is untouched. The total length is always returned.
; i.E. if the provided memory is 2 bytes (including
; the 0-byte) but the number would require 5 bytes, then
; 5 is returned and the output buffer is not modified.
;
; TRASHED: A0, A1
;
; #####################################################

	section .text,code

_toDecimalU8::
toDecimalU8::
	and.w	#$ff,d0

_toDecimalU16::
toDecimalU16::
    move.l  d2,-(a7)

    move.l  a7,a1
    move.l  a1,d2		; Remember the length including the 0-byte
    move.b  #0,-(a1)

    tst.w	d0
    beq.s	.print

.loop:
    divu.w  #10,d0
    swap    d0

.print:
    add.b   #'0',d0
    move.b  d0,-(a1)
    clr.w   d0
    swap    d0
    bne.s   .loop

    sub.l   a1,d2		; Calculate the length, including the 0-byte)
    cmp.w   d2,d1		; Provided buffer is big enough?
    blt.s   .Done

.strcpy:
    move.b  (a1)+,(a0)+
    cmp.l   a1,a7
    bne.s   .strcpy

.Done:
    move.l  d2,d0
    subq    #1,d0
    move.l  (a7)+,d2
    rts
