; #####################################################
; Converts a 32 bit number to a string without
; 0-byte. The number of required output length is
; always returned, even if the buffer is to small. In
; this case, the output buffer is not overwritten but
; the return value indicates how big the buffer should
; have been.
; This function creates no leading zeroes, so a value
; of #1 will return 1, a value of i.e. #37 returns
; 2 and so on.
;
; INPUT:
;	D0.l: Value to convert
;	A0: Address of output buffer
;	A1: End of output buffer
;
; RETURN: D0.L: Number of required bytes
;
; Modified: D0-D2/A0/A2/A3
;
; #####################################################

	section .text,code

toBase32::
    moveq   #5,d1
    moveq   #$1f,d2
    bne.s   toString

toHex::
    moveq   #4,d1
    moveq   #$f,d2
    bne.s   toString

toOct::
    moveq   #3,d1
    moveq   #7,d2
    bne.s   toString

toBase4::
    moveq   #2,d1
    moveq   #3,d2
    bne.s   toString

toBinary::
    moveq   #1,d1
    moveq   #1,d2

; ##############################################
; D0.L = Value
; D1.L = Number of Bits to shift
; D2.L = Bitmask for digit
; A0 = start of output
; A1 = End of outputbuffer
; Return: Number of characters written.
;
; Modified: D0-D3/A0/A2/A3
;

toString::
    lea     numericChar,a3
    moveq   #32,d3
    sub.l   d3,a7
    move.l  a7,a2

.loop:
    move.b  d0,d3
    and.b   d2,d3

    move.b  0(a3,d3.w),(a2)+

    lsr.l   d1,d0
    bne.s   .loop

    move.l  a2,d0
    sub.l   a7,d0			; Length of string == return value

	moveq	#0,d3
	cmp.l	d3,a0			; nullptr - we are done.
	beq.s	.Exit
	
    move.w  d0,d3			; Number of characters to copy

.strcpy:
    cmp.l   a0,a1
    beq.s   .Exit			; Don't overrun buffer
    move.b  -(a2),(a0)+
    subq    #1,d3
    bne.s   .strcpy

.Exit:
    moveq   #32,d3
    add.l   d3,a7

    rts
