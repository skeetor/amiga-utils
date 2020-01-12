; #####################################################
; Converts an signed 32 bit number to a decimal c-string.
;
; INPUT:
;	A0: Address of output buffer
;	D0.l: Number to convert
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

_toDecimalS32::
toDecimalS32::
    swap    d0
    bne.s   .Do32
    swap    d0
    jmp     toDecimalS16

.Do32:
    swap    d0
    move.l  d2,-(a7)
    move.w  d3,-(a7)

    move.l  a7,a1
    sub.l   #12,a7

    move.l  a1,d2		; Remember the length including the 0-byte
    move.b  #0,-(a1)
    
    tst.l   d0
    beq.s   .print

    btst    #31,d0
    beq.s   .loop

    move.w  #1,d3
    neg.l   d0

.loop:
    moveq   #10,d1
    jsr     div32s

.print:
    add.b   #'0',d1
    move.b  d1,-(a1)
    tst.w   d0
    bne.s   .loop

    tst.w   d3
    beq.s   .length
    move.b  #'-',-(a1)

.length:
    add.l   #12,a7
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

    move.w  (a7)+,d3
    move.l  (a7)+,d2
    rts
