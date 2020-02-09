
; Converts a string to an unsigned 32 bit number.
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
;	D0.L: Value of the string. If D1 Error is
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
;			buffer end has been reached), 
; Modified: D0-D6, A0/A1
;
; #####################################################

	section .text,code

stringToSLong::

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
    beq.s   .stringToLong

    cmp.b   #'-',d2
    beq.s   .Negativ

    ; Character is unknown so it
    ; should be checked by the conversion
    subq    #1,a0
    bra.s   .stringToLong

.Negativ:
    moveq   #0,d5

.stringToLong:
    moveq   #0,d4
    move.l  #$80000000,d3

    move.l  d5,-(sp)
    jsr     stringToLong
    move.l  (sp)+,d5

    tst.w   d5
    bne.s   .Done

    neg.l   d0

.Done:
    rts

_stringToULong::
	; state pointer passed in A1
	movem.l	d2-d6/a1,-(sp)

	exg		d1,d0

	jsr		stringToULong

	movem.l	(sp)+,d2-d6/a1

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
	
stringToULong::

    moveq   #0,d2
    cmp.l   a0,d2
    beq.w   scError         ; nullptr

    move.w  d1,d2
	beq.w	scError          ; Buffersize is 0
    
    move.l  a0,a1
    add.l   d2,a1

    move.b  (a0)+,d1
    cmp.b   #'+',d1
    beq.s   .ToLong

    subq    #1,a0

.ToLong:
    moveq   #0,d5
    moveq   #0,d4
    moveq   #0,d3
    subq.l  #1,d3

; ##########################################
; D0.W: Base
; D3.L: Cutoff
; D4.B: Limit
; D5.w: 1 if positive, 0 if negative
; A0.L: Stringstart
; A1.L: End of Stringbuffer
stringToLong::
    move.w  d0,d2
    cmp.w   #2,d0
    blt.w   scError
    bne.s   .NotBase2

    moveq   #1,d3
    moveq   #32,d4
    jmp     stringToNumber

.NotBase2:
    cmp.w   #16,d0
    bne.s   .NotBase16

    moveq   #4,d3
    moveq   #8,d4
    jmp     stringToNumber

.NotBase16:
    cmp.w   #8,d0
    bne.s   .NotBase8

    moveq   #3,d3
    moveq   #12,d4
    jmp     stringToNumber

.NotBase8:
    cmp.w   #4,d0
    bne.s   .NotBase4

    moveq   #2,d3
    moveq   #16,d4
    jmp     stringToNumber

.NotBase4:
    cmp.w   #32,d0
    bne.s   .NotBase32

    moveq   #5,d3
    moveq   #8,d4
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

    move.l  d2,d1
    move.l  d3,d0
    jsr     div32u

    move.l  d0,d3
    move.l  d1,d4   ; limit
    beq.s   .Start

    sub.l   d5,d4
    
.Start:
    moveq   #0,d5
    move.l  d5,d6

.DoConvert:
    cmp.l   a0,a1
    ble.s   .Exit           ; Here we are really done, because we
                            ; may not read beyond the buffer length

    ; From here we alread read the next char, so if it leads
    ; to an error, we have to roll back the pointer so the
    ; caller can decide if this is an error or not.
    move.b  (a0)+,d6
    beq.s   .Done

    cmp.b   #'9',d6
    bhi.s   .Upper

    cmp.b   #'0',d6
    blt.s   .EndOfNumber

    sub.b   #'0',d6
    bra.s   .CheckLimits

.Upper:
    cmp.b   #'Z',d6
    bhi.s   .Lower

    cmp.b   #'A',d6
    blt.s   .EndOfNumber

    sub.b   #'A'-10,d6
    bra.s   .CheckLimits

.Lower:
    cmp.b   #'z',d6
    bhi.s   .EndOfNumber

    cmp.b   #'a',d6
    blt.s   .EndOfNumber

    sub.b   #'a'-36,d6

.CheckLimits:
    cmp.b   d6,d2
    ble.s   .EndOfNumber   ; Character not allowed for this base

    cmp.l   d5,d3           ; Cutoff reached?
    bhi.s   .Valid
    blt.s   .Overflow

    ; Cutoff reached, so we check 
    ; if the number is within the limit
    cmp.b   d6,d4           ; Limit reached
    blt.s   .Overflow

.Valid:
    move.l  d5,d0
    move.l  d2,d1
    jsr     mult32u
    move.l  d0,d5
    add.l   d6,d5
    
    bra.s   .DoConvert

.Done:
    subq    #1,a0

.Exit:
    move.l  d5,d0
    moveq   #0,d1
    rts

.Overflow:
    move.l  d5,d0
    subq    #1,a0
    moveq   #1,d1
    rts

.EndOfNumber:
    move.l  d5,d0
    subq    #1,a0
    moveq   #2,d1
    rts

scError:
    moveq   #-1,d0
    moveq   #3,d1
    rts
