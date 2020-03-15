
; Converts a string to an unsigned 16 bit number.
; The number is converted until either a non-numeric
; character (for the given base) is encountered or
; the buffer limit is reached.
;
; Note, that the length of the string is not required
; to end with a c-string 0 byte, so this function can
; also be used to convert numbers embedded inside a
; string (i.E. fixed width record or other text).
;
; A leading '+' or '-' is considered as valid part
; of the number, but no whitespaces between.
;
; INPUT:
;	A0: Address of string to convert.
;	D0.w: Base (2-62)
;	D1.l: length of string to convert
;
; RETURN:
;	D0.W: Value of the string. If D1 Error is
;		  is set, this value is always -1.
;	D1.L:	0 == Converted number is correct
;			1 == An overflow would occur
;				 In this case, the returned number
;				 is the last known value without overflow.
;				 This might not be an error if the string
;				 is embedded, but the caller has to decide.
;			2 == Invalid character encountered.
;				 The return value contains the number
;				 up to this character. Caller has to
;				 decide if this is an error or not.
;			3 == Error. Invalid Base, Buffersize == 0
;				 or nullptr.
;
;	A0.L:	Points to the last not converted character.
;			This is either the end of the buffer (if the
;			buffer end has been reached), or the first
;			character after the last digit.
;
; Modified: D0-D5, A0/A1
;
; #####################################################

	section .text,code

stringToSWord::

    moveq   #0,d2
    cmp.l   a0,d2
    beq.w   scError         ; nullptr

    move.w  d1,d2
	beq.w	scError          ; Buffersize is 0
    
    move.l  a0,a1
    add.l   d2,a1

    moveq   #1,d5
    move.b  (a0)+,d2
    cmp.b   #'+',d2
    beq.s   .stringToWord

    cmp.b   #'-',d2
    beq.s   .Negativ

    ; Character is unknown so it
    ; should be checked by the conversion
    subq    #1,a0
    bra.s   .stringToWord

.Negativ:
    moveq   #0,d5

.stringToWord:
    moveq   #0,d4
    move.l  #$8000,d3

    jsr     stringToWord

    tst.w   d5
    bne.s   .Done

    neg.w   d0

.Done:
    rts

_stringToUWord::
	; state pointer passed in A1
	movem.l	d2-d5/a1,-(sp)

	exg		d1,d0

	jsr		stringToULong

	movem.l	(sp)+,d2-d5/a1

	cmp.l	#0,a1
	beq.s	.CheckEndbuffer

	move.l	d1,(a1)

.CheckEndbuffer:
	moveq	#0,d1
	move.l	4(sp),a1
	cmp.l	d1,a1
	beq.s	.Done

	move.l	a0,(a1)

.Done:
	rts

stringToUWord::

    moveq   #0,d2
    cmp.l   a0,d2
    beq.w   scError         ; nullptr

    move.w  d1,d2
	beq.w	scError          ; Buffersize is 0
    
    move.l  a0,a1
    add.l   d2,a1

    move.b  (a0)+,d1
    cmp.b   #'+',d1
    beq.s   .ToWord

    subq    #1,a0

.ToWord:
    moveq   #0,d5
    moveq   #0,d4
    moveq   #0,d3
    subq.w  #1,d3

; ##########################################
; D0.W: Base
; D3.L: Cutoff
; D4.B: Limit
; D5.w: 1 if positive, 0 if negative
; A0.L: Stringstart
; A1.L: End of Stringbuffer
stringToWord:
    move.w  d0,d2
    cmp.w   #2,d0
    blt.w   scError
    bne.s   .NotBase2

    moveq   #1,d3
    moveq   #16,d4
    jmp     stringToNumber

.NotBase2:
    cmp.w   #16,d0
    bne.s   .NotBase16

    moveq   #4,d3
    moveq   #4,d4
    jmp     stringToNumber

.NotBase16:
    cmp.w   #8,d0
    bne.s   .NotBase8

    moveq   #3,d3
    moveq   #6,d4
    jmp     stringToNumber

.NotBase8:
    cmp.w   #4,d0
    bne.s   .NotBase4

    moveq   #2,d3
    moveq   #8,d4
    jmp     stringToNumber

.NotBase4:
    cmp.w   #32,d0
    bne.s   .NotBase32

    moveq   #5,d3
    moveq   #4,d4
    jmp     stringToNumber

.NotBase32:
    cmp.w   #62,d0
    bhi.w   scError

.CalcLimit:
    ; Calculate numeric limit and cutoff
    ; This is done by using $ffff as a start
    ; as this is the highest number a word can hold.
    ; If the modulo is zero, then $ffff is multiple
    ; of the base. If not, then the calculated number
    ; plus the modulo, is the highest value that can
    ; be held without overflowing. 
    move.l  d3,d1
    divu    d0,d1
    swap    d1
    move.w  d1,d4   ; limit
    beq.s   .Start

    sub.w   d5,d4
    swap    d1
    move.w  d1,d3   ; cutoff

.Start:
    moveq   #0,d0
    move.l  d0,d1

.DoConvert:
    cmp.l   a0,a1
    ble.s   .Exit           ; Here we are really done, because we
                            ; may not read beyond the buffer length

    ; From here we alread read the next char, so if it leads
    ; to an error, we have to roll back the pointer so the
    ; caller can decide if this is an error or not.
    move.b  (a0)+,d1
    beq.s   .Done

    cmp.b   #'9',d1
    bhi.s   .Upper

    cmp.b   #'0',d1
    blt.s   .EndOfNumber

    sub.b   #'0',d1
    bra.s   .CheckLimits

.Upper:
    cmp.b   #'Z',d1
    bhi.s   .Lower

    cmp.b   #'A',d1
    blt.s   .EndOfNumber

    sub.b   #'A'-10,d1
    bra.s   .CheckLimits

.Lower:
    cmp.b   #'z',d1
    bhi.s   .EndOfNumber

    cmp.b   #'a',d1
    blt.s   .EndOfNumber

    sub.b   #'a'-36,d1

.CheckLimits:
    cmp.b   d1,d2
    ble.s   .EndOfNumber   ; Character not allowed for this base

    cmp.l   d0,d3           ; Cutoff reached?
    bhi.s   .Valid
    blt.s   .Overflow

    ; Cutoff reached, so we check 
    ; if the number is within the limit
    cmp.b   d1,d4           ; Limit reached
    blt.s   .Overflow

.Valid:
    mulu    d2,d0
    add.l   d1,d0

    bra.s   .DoConvert

.Done:
    subq    #1,a0

.Exit:
    moveq   #0,d1
    rts

.Overflow:
    subq    #1,a0
    moveq   #1,d1
    rts

.EndOfNumber:
    subq    #1,a0
    moveq   #2,d1
    rts

scError:
    moveq   #-1,d0
    moveq   #3,d1
    rts

; ###############################################
; Convert a power of base 2 string to a word/long.
; This function does not check parameters, but
; is faster then using the generic conversion
;
; A0: start of string
; A1: End of string
; D2.L: Base
; D3.L: Bits to shift
; D4.B: Number of shifts

stringToNumber::
    moveq   #0,d0
    moveq   #0,d1
    addq    #1,d4

.DoConvert:
    cmp.l   a0,a1
    ble.s   .Exit           ; Here we are really done, because we
                            ; may not read beyond the buffer length

    move.b  (a0)+,d1
    beq.s   .Exit

    cmp.b   #'9',d1
    bhi.s   .Upper

    cmp.b   #'0',d1
    blt.s   .EndOfNumber

    sub.b   #'0',d1
    bra.s   .CheckLimits

.Upper:
    cmp.b   #'Z',d1
    bhi.s   .Lower

    cmp.b   #'A',d1
    blt.s   .EndOfNumber

    sub.b   #'A'-10,d1
    bra.s   .CheckLimits

.Lower:
    cmp.b   #'z',d1
    blt.s   .EndOfNumber

    cmp.b   #'a',d1
    blt.s   .EndOfNumber

    sub.b   #'a'-36,d1

.CheckLimits:
    cmp.b   d1,d2
    ble.s   .EndOfNumber    ; Character not allowed for this base
    
.Valid:
    ; We check this here, so we can detect a possible overflow
    subq.w  #1,d4
    beq.s   .Overflow

    lsl.l   d3,d0
    add.l   d1,d0
    bra.s   .DoConvert
    
.Exit:
    moveq   #0,d1
    rts

.Overflow:
    subq    #1,a0
    moveq   #1,d1
    rts

.EndOfNumber:
    subq    #1,a0
    moveq   #2,d1
    rts

.Error:
    moveq   #-1,d0
    moveq   #3,d1
    rts
