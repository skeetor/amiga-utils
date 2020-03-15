	section .text,code

; #####################################################
; Fill memory with a given byte.
;
; INPUT:
;	D0: Value to be set (byte)
;	D1: size of buffer to fill
;	A0: buffer to be filled, The buffer may also be on
;		an uneven address but will take a speed penalty.
;
; RETURN:
; 	D0: Returns the input pointer as required by C
;
; MODIFIED: D1,A0,A1
;
; #####################################################

_memset::
	move.l	4(sp),a0
	move.l	8(sp),d0
	move.l	12(sp),d1

memset::
    movem.l     d2/a0,-(sp)

    move.l      a0,d2
    btst        #0,d2
    beq.s       .IsEven

    move.b      d0,(a0)+
    subq        #1,d1

.IsEven:
    cmp.l       #8,d1
    bge.s       .Do8
    bra.s       .Do1

.Do8:
    movem.l     d1,-(sp)
    cmp.l       #16,d1
    bge.s       .Do16

    jsr         memset8
    bra.s       .DoRemainder

.Do16:
    movem.l     d3/d4,-(sp)
    jsr         memset16
    movem.l     (sp)+,d3/d4

.DoRemainder:
    ; We want to preserve D1 as it still contains the fillbyte
    ; so we restore this to D2 which is currently
    ; a scratch register
    movem.l    (sp)+,d2

    sub.l       d0,d2
    beq.s       .Done

    move.l      d1,d0
    move.l      d2,d1
    move.l      a1,a0

.Do1:
    bsr.s       memset1

.Done:
    movem.l     (sp)+,d2/a0
    rts

; #####################################################
; Fill memory in 16 byte chunks with a given byte. If
; the size is not a muiltiple of 16, then memory is
; filled only until the last chunk. the remaining bytes
; have to be filled by the caller.
;
; Note, that the pointer MUST be on an even address.
;
; INPUT:
;	D0: Value to be set (byte)
;	D1: size of buffer to fill
;	A0: buffer to be filled
;
; RETURN:
; 	D0: Number of bytes filled actually filled.
;	A0: Unchanged
;	A1: Points to first byte after last filled byte
;
; MODIFIED: D0-D4,A1
;
; #####################################################
memset16::
    move.l      d1,d4
    lsr.l       #4,d4
    beq.s       .Done

    and.b       #$f0,d1
    add.l       d1,a0
    move.l      a0,a1

    move.b      d0,d1
    lsl.w       #8,d0
    move.b      d1,d0
    move.w      d0,d1
    swap        d0
    move.w      d1,d0
    move.l      d0,d1
    move.l      d0,d2
    move.l      d0,d3

.loop:
    movem.l     d0-d3,-(a0)
    subq        #1,d4
    bne.s       .loop

.Done:
    move.l      a1,d0
    sub.l       a0,d0
    rts

; #####################################################
; Fill memory in 8 byte chunks with a given byte. If
; the size is not a muiltiple of 8, then memory is
; filled only until the last chunk. the remaining bytes
; have to be filled by the caller.
;
; Note, that the pointer MUST be on an even address.
;
; INPUT:
;	D0: Value to be set (byte)
;	D1: size of buffer to fill
;	A0: buffer to be filled
;
; RETURN:
; 	D0: Number of bytes filled actually filled.
;	A0: Unchanged
;	A1: Points to first byte after last filled byte
;
; MODIFIED: D0-D2,A1
;
; #####################################################
memset8::
    move.l      d1,d2
    lsr.l       #3,d2
    beq.s       .Done

    and.b       #$f8,d1
    add.l       d1,a0
    move.l      a0,a1

    move.b      d0,d1
    lsl.w       #8,d0
    move.b      d1,d0
    move.w      d0,d1
    swap        d0
    move.w      d1,d0
    move.l      d0,d1

.loop:
    movem.l     d0-d1,-(a0)
    subq        #1,d2
    bne.s       .loop

.Done:
    move.l      a1,d0
    sub.l       a0,d0
    rts

memset1:
.loop:
    move.b  d0,(a0)+
    subq    #1,d1
    bne.s   .loop
    rts
