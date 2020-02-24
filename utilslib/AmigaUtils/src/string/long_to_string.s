; #####################################################
; Converts an signed/unsigned 32 bit number to a string. No
; 0 byte is added and no leading zeroes are included in
; the string.
; The number of required bytes is always returned, but
; if the buffer is to small, it will not be overwritten.
; In this case the buffer is truncated and will contain
; an invalid value, so the caller should make sure that
; enough memory is provided.
; If the base is invalid, -1 is returned.
;
; For bases with power of 2 a more specialiced 
; conversion is used, as these are much faster.
; These are 2/4/8/16 and 32.
;
; Examples (in cycles):
; #3866565 Base 2 = 8316/2480 / 16 = 2614/852 / 32 = 2270/816
;
; INPUT:
;	A0: Address of output buffer
;	D0.l: Value to convert
;	D1.l: size of outputbuffer
;	D2.l: base (2-62)
;
; RETURN: D0.L: Length of string
;
; Modified: D0-D3, A0-A3
;
; #####################################################

	section .text,code

_sLongToString::
	movem.l	d2-d3/a2-a3,-(sp)

	moveq	#0,d2
	move.w	a1,d2

	jsr		sLongToString

	movem.l	(sp)+,d2-d3/a2-a3
	rts

_uLongToString::
	movem.l	d2-d3/a2-a3,-(sp)

	moveq	#0,d2
	move.w	a1,d2

	jsr		uLongToString

	movem.l	(sp)+,d2-d3/a2-a3
	rts

sLongToString::
    btst    #31,d0
    beq.s   uLongToString

    neg.l    d0

    ; If this is a nullptr we can not add the sign.
    exg     d0,a0
    tst.L   d0
    exg     d0,a0
    beq.s   .AddLength

    ; If bufferlength is zero we can not add the sign
    tst.l   d1
    beq.s   .AddLength

    move.b  #'-',(a0)+
    subq    #1,d1
    
.AddLength:
    jsr     uLongToString
    addq    #1,d0
    beq.s   .Error          ; If an error is returned we must restore it
    rts

.Error:
    subq    #1,d0
    rts

uLongToString::
    ; If highword is zero we can use
    ; the faster 16 bit conversion
    move.l  d0,d3
    swap    d3
    tst.w   d3
    bne.s   .LongString

    jmp     uWordToString

.LongString:
    move.l  a0,a1
    add.l   d1,a1           ; End of buffer

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
    lea     numericChar,a3
    moveq   #32,d1
    sub.l   d1,a7
    move.l  a7,a2
    
.loop:
    move.l  d2,d1           ; Base
    jsr     div32u
 
    move.b  0(a3,d1.w),(a2)+

.Start:
    tst.l   d0
    bne.s   .loop

    move.l  a2,d0
    sub.l   a7,d0			; Length of string == return value
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
