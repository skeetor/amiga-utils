; #####################################################
; Converts a signed/unsigned 16 bit number to a string. No
; 0 byte is added and no leading zeroes are included in
; the string.
; The number of required bytes is always returned, but
; if the buffer is to small, it will not be overwritten.
; In this case the buffer is truncated and may contain
; an invalid string, so the caller should make sure that
; enough memory is provided.
; If the base is invalid, -1 is returned.
;
; For bases with power of 2 a more specialiced 
; conversion is used, as these are much faster.
; These are 2/4/8/16 and 32.
;
; Examples (in cycles):
; #65535 Base 2 = 3618/1602 / 16 = 1114/562 / 32 = 1126/636
;
; INPUT:
;	A0: Address of output buffer
;	D0.w: Value to convert, upper word must be 0
;	D1.l: size of outputbuffer
;	D2.w: base (2-62)
;
; RETURN:
; D0.L: Length of string
; A0: Points to the first byte after the number string
;
; Modified: D0-D2, A0-A3
;
; #####################################################

	section .text,code

_sWordToString::

	movem.l	d2/a2-a3,-(sp)

	moveq	#0,d2
	move.w	a1,d2

	jsr		uWordToString

	movem.l	(sp)+,d2/a2-a3
	rts

_uWordToString::

	movem.l	d2/a2-a3,-(sp)

	moveq	#0,d2
	move.w	a1,d2

	jsr		uWordToString

	movem.l	(sp)+,d2/a2-a3
	rts

sWordToString::
    btst    #15,d0
    beq.s   uWordToString

    neg.w    d0

    ; If this is a nullptr we can not add the sign.
    exg     d0,a0
    tst.w   d0
    exg     d0,a0
    beq.s   .AddLength

    ; If bufferlength is zero we can not add the sign
    tst.l   d1
    beq.s   .AddLength

    move.b  #'-',(a0)+
    subq    #1,d1
    
.AddLength:
    jsr     uWordToString
    addq    #1,d0
    beq.s   .Error          ; If an error is returned we must restore it
    rts

.Error:
    subq    #1,d0
    rts

uWordToString::

    move.l  a0,a1
    add.l   d1,a1           ; End of buffer

	moveq	#0,d1
    move.w  d0,d1
	move.l	d1,d0

	bne.s	.CheckBase
    jmp		toBinary

.CheckBase:
    cmp.w   #16,d2
    bne.s   .NotBase16
    jmp     toHex

.NotBase16:
    cmp.w   #2,d2
    bne.s   .NotBase2
    jmp     toBinary

.NotBase2:
    blt     .Error
    cmp.w   #8,d2
    bne.s   .NotBase8
    jmp     toOct

.NotBase8:
    cmp.w   #32,d2
    bne.s   .NotBase32
    jmp     toBase32
    
.NotBase32:
    cmp.w   #4,d2
    bne.s   .MaxBase
    jmp     toBase4

.MaxBase:
    cmp.w   #62,d2
    bhi.s   .Error

.DoConvert:
    moveq   #32,d1
    sub.l   d1,a7
    move.l  a7,a2

    lea     numericChar,a3

    bra.s   .Start

.loop:
    divu.w  d2,d1
    move.w  d1,d0
    swap.w  d1			; Modulo

    move.b  0(a3,d1.w),(a2)+

.Start:
    moveq   #0,d1
    move.w  d0,d1
    bne.s   .loop

    move.l  a2,d0
    sub.l   a7,d0		; Length of string == return value

    moveq   #0,d1
	cmp.l	d1,a0		; if nullptr we don't copy and can return
	beq.s	.Done

    move.l  d0,d1

.strcpy:
    cmp.l   a0,a1
    ble.s   .Done
    move.b  -(a2),(a0)+
    subq    #1,d1
    bne.s   .strcpy

.Done:
    moveq   #32,d1
    add.l   d1,a7
    rts

.Error:
    moveq   #-1,d0
    rts

	section .data,data

_numericChar::
numericChar:: dc.b '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz',0
