; ************************************************************
; Print formatted string (similar to the snprintf).
; If the buffer is to small, the buffer is not exceeded
; and the number of required characteres are still
; returned (without the 0-byte).
; Even if the buffer is to small, the resulting string
; is guranteed to be 0-terminated, except when the supplied
; buffersize is 0 or a nullpointer.
; Passing a nullpointer or 0 size is still valid, as this
; will return the total required length of the output
; buffer.
;
; INPUT:  
;	buffer: Outputbuffer
;	len:	Length of outputbuffer
;	format: Formatstring
;	args:	Variable args for the format parameters
;
; Format arguments are pushed on the stack. All arguments
; are assumed 32 bit on stack.
;
;	Format:
;		%(-N*l)b - binary value
;		%c - single 8 bit character
;		%(-N*l)d - signed decimal integer
;		%(-N*)s - string. If the pointer is null "[null]" is inserted.
;		%(-N*l)u - unsigned 32 bit integer
;		%(-N*l)x - 16/32 bit hex value lowercase
;		%(-N*l)X - 16/32 bit hex value uppercase
;		%p - pointer
;		%n - store the number of characters already written (pointer to uint32_t).
;		%% - '%' character
;
;		Floating point formatters are not implemented and will always
;		print 'INF'. They are only placeholders for compatibillity with
;		standard printf familly. An argument of 32 bit still has to be
;		pushed.
;
;		%f - Floatingpoint.
;		%e - Floatingpoint.
;
;		\n - $0d	CR
;		\r - $0a	LF
;		\t - $08	TAB
;		\\ - '\'
;
; Format modifiers:
;  *	- The width of the a field is determined by a 32 bit parameter.
; <N>	- A number that specifies the minimum width of the field.
;		  If the number starts with '0' leading zeroes are padded.
; -	- Field is right aligned, defsault is left alignment
; l	- Argument is long (32 bit value), default is 16.
;
;
; IMPORTANT NOTE: Remember that arguments for the format
; values, have to be pushed in reverse order (from right to left).
; Example: "Test %b %lX %d", 14u, $AB, 32
;
; push  32
; push  $ab
; push  14
; jsr	formatString
;
; ***********************************************************
;
; INPUT:
;	D0: Length of outputbuffer
;	D1:	Pointer to first parameter on stack
;	A0: Outputbuffer
;	A1: Formatstring
;
; RETURN: D0: Length of string (without 0-byte). If format is
;		a nullptr, -1 is returned.
;
; Modified: D0, A0-A1
;
; ************************************************************

	section .text,code


SAVE_CHAR   MACRO
    cmp.l   a3,a4
    ble.s   .addCount\@

    move.b  \1,(a3)+
    
.addCount\@:
    addq    #1,d7

    ENDM

STR_MODE_SET    	equ 	31
STR_CONTROL_MODE	equ 	30
STR_FORMAT_MODE		equ		29
STR_ZERO_FILL		equ		28
STR_ALIGN_RIGHT		equ		27
STR_PARAM_32		equ		26
STR_SIGNED			equ		25
STR_LOWERCASE		equ		24


NUMERIC_BUFFER_SIZE equ     34

_formatString::
	move.l	a0,-(sp)
	lea		8(a7),a0
	move.l	a0,d1
	move.l	(sp)+,a0

_vFormatString::
formatString::

    movem.l a5,-(sp)

    sub.l   a5,a5
    cmp.l   a5,a1
    bne.s   .FormatOk

    movem.l (sp)+,a5
    moveq   #-1,d0
    rts

.FormatOk:
    ; Save all registers. d2-d4+d5 is used by the number conversions

    ; A5 is already saved above, so we don't save it here again, but we
    ; we restore it at the end.    
    movem.l d2-d7/a2-a4,-(sp)
    move.l	d1,a5   ; Argument base pointer

    movem.l a0,-(sp)    ; Remember original outputbuffer for the final 0-byte
    moveq   #NUMERIC_BUFFER_SIZE,d7
    sub.l   d7,a7

	moveq	#0,d7       ; Charcount
	moveq	#0,d6       ; Mode+char
	moveq	#0,d5       ; fieldWidth

	move.l	a0,a3       ; Outputbuffer
	move.l	a0,a4
    cmp.l   d5,a3
    beq.s   .Null       ; In case of a nullptr both should be the same

	add.l	d0,a4       ; End of outputbuffer

.Null:
	move.l	a1,a2       ; FormatString
	bra.w	.NextChar

.loop:

; ******************************************************
; Control mode
	btst	#STR_CONTROL_MODE,d6
	beq.s	.CheckFormatMode

.TabChar:
    cmp.b   #'t',d6
    bne.s   .NewlineChar

    moveq   #0,d6           ; Clear mode
    move.b  #$09,d6
    bra.w   .StoreChar

.NewlineChar:
    cmp.b   #'n',d6
    bne.s   .ReturnChar

    moveq   #0,d6           ; Clear mode
    move.b  #$0d,d6
    bra.w   .StoreChar

.ReturnChar:
    cmp.b   #'r',d6
    bne.w   .StoreChar      ; Unknown character gets just stored as is.

    moveq   #0,d6           ; Clear mode
    move.b  #$0a,d6
    bra.w   .StoreChar

; ******************************************************
; Format mode
.CheckFormatMode:
	btst	#STR_FORMAT_MODE,d6
	beq.w	.DoChar

    cmp.b   #'l',d6
    bne.s   .CheckNumber

    bset    #STR_PARAM_32,d6
    bra.w   .NextChar

.CheckNumber:
    cmp.b   #'9',d6
    bhi.s   .CheckAlign

    cmp.b   #'0',d6
    blt.s   .CheckAlign
    bne.s   .FieldWidth

    bset    #STR_ZERO_FILL,d6
    bset    #STR_ALIGN_RIGHT,d6

.FieldWidth:
    subq    #1,a2           ; Readjust number pointer to grab the first char as well

    move.l  a2,a0
    move.l  #10,d0
    moveq   #-1,d1          ; Length shouldn't matter...
    jsr     stringToUWord
    move.l  d0,d5
    move.l  a0,a2
    
    bra.w   .NextChar

.CheckAlign:
    cmp.b   #'-',d6
    bne.s   .CheckString

    bset    #STR_ALIGN_RIGHT,d6
    bra.w   .NextChar
    
.CheckString:
    cmp.b   #'s',d6
    bne.s   .UnsignedDecimal

    move.l  (a5)+,d0
    bne.s   .StringCopy

    move.l  #NullString,d0

.StringCopy:
    move.l  d0,a0
    moveq   #-1,d0          ; Length unknown
    jsr     insertString
    moveq   #0,d6
    bra.w   .NextChar

.UnsignedDecimal:
    cmp.b   #'u',d6
    bne.s   .SignedDecimal

    lea     insertUnsignedNumber,a1

.DoDecimal:
    moveq   #10,d2              ; Base 10

.DoNumber:
    move.l  a7,a0
    move.l  (a5)+,d0
    moveq   #NUMERIC_BUFFER_SIZE,d1

    jsr     (a1)

    moveq   #0,d6
    bra.w   .NextChar

.SignedDecimal:
    cmp.b   #'d',d6
    bne.s   .HexUpper

    lea     insertSignedNumber,a1
    bra.s   .DoDecimal

.HexUpper:
    cmp.b   #'X',d6
    beq.s   .ToHex
    cmp.b   #'x',d6
    beq.s   .LowerHex
    cmp.b   #'p',d6
    bne.s   .Binary

    SAVE_CHAR #'0'
    SAVE_CHAR #'x'

    or.l    #(1<<STR_ZERO_FILL)|(1<<STR_PARAM_32)|(1<<STR_ALIGN_RIGHT),d6
    move.l  #8,d5
    
.LowerHex:
    bset    #STR_LOWERCASE,d6
    
.ToHex:
    moveq   #16,d2              ; Base 16

    lea     insertUnsignedNumber,a1
    bra.s   .DoNumber
    
.Binary:
    cmp.b   #'b',d6
    bne.s   .SingleChar

    moveq   #2,d2              ; Base 2

    lea     insertUnsignedNumber,a1
    bra.s   .DoNumber
	
.SingleChar:
    cmp.b   #'c',d6
    bne.s   .NumberParam

    move.l  (a5)+,d0
    moveq   #0,d6
    move.b  d0,d6
    bra.s   .StoreChar

.NumberParam:
    cmp.b   #'*',d6
    bne.s   .ReportSize

    move.l  (a5)+,d5
    bra.s   .NextChar

.ReportSize:
    cmp.b   #'n',d6
    bne.s   .Float

    moveq   #0,d6
    move.l  (a5)+,d0
    beq.s   .NextChar           ; Ignore if nullptr

    move.l  d0,a0
    move.l  d7,(a0)
    bra.s   .NextChar

.Float:
    cmp.b   #'e',d6
    beq.s   .INFText
    cmp.b   #'f',d6
    beq.s   .INFText

    moveq   #0,d6
    bra.s   .StoreChar
    
.INFText:
    addq    #4,a5
    moveq   #0,d6

	SAVE_CHAR #'I'
	SAVE_CHAR #'N'
	SAVE_CHAR #'F'

    bra.s   .NextChar  

; ******************************************************
; Unknown characters
.DoChar:
    cmp.b   #'%',d6
    bne.s   .CheckBackSlash

    bset    #STR_MODE_SET,d6
    bset    #STR_FORMAT_MODE,d6
    moveq   #0,d5

    bra.s   .NextChar

.CheckBackSlash:
    cmp.b   #'\\',d6
    bne.s   .StoreChar

    bset    #STR_MODE_SET,d6
    bset    #STR_CONTROL_MODE,d6
    bra.s   .NextChar

.StoreChar:
	SAVE_CHAR   d6

.NextChar:
	move.b	(a2)+,d6
	beq.s  	.AddZeroByte

    btst    #STR_MODE_SET,d6
	beq.s	.DoChar
    bra.w   .loop
    
.AddZeroByte:
    moveq   #NUMERIC_BUFFER_SIZE,d0
    add.l   d0,a7
    movem.l (sp)+,d0            ; Outputbuffer

    ; If outputbuffer was a nullptr we can't store the 0-byte ...
    tst.l   d0
    beq.s   .Done

    ; ... and also not if the buffersize was 0
    cmp.l   d0,a4
    beq.s   .Done
    
    cmp.l   a3,a4
    bgt.s   .storeZero

    subq    #1,a3

.storeZero:
    move.b  #0,(a3)+

.Done:
    move.l  d7,d0
    movem.l (sp)+,d2-d7/a2-a5

	rts

; ################################################
; D0.L: Length of string or -1 if unwknown
; A0: Pointer to String
;
; Other registers as used from the main function.
; D5-D7/A2-A4
;
insertString:
    tst.l   d5
    beq.s   .CopySource

    btst    #STR_ALIGN_RIGHT,d6
    beq.s   .CopySource

    moveq   #-1,d1
    cmp.l   d1,d0
    bne.s  .CheckAlign

.SourceLen:
    move.l  a0,a1

.SourceStrLen:
    tst.b   (a1)+
    bne.s   .SourceStrLen

    move.l  a1,d0
    sub.l   a0,d0
    subq    #1,d0

    ; Fill the width with leading blanks or zeroes
.CheckAlign:
    sub.l  d0,d5
    ble.s  .FillDone

    move.b  #'0',d6
    btst    #STR_ZERO_FILL,d6
    bne.s   .FillLeading

    move.b  #' ',d6

.FillLeading:
    SAVE_CHAR   d6
    subq    #1,d5
    bne.s   .FillLeading
    
.FillDone:
    moveq   #0,d5

.CopySource:
    move.l  a0,d0

.CopyLoop:
    move.b  (a0)+,d6
    beq.s   .FillTrailing

    SAVE_CHAR   d6
    bra.s   .CopyLoop

.FillTrailing:
    tst.l   d5
    beq.s   .Done

    add.l   d0,d5
    sub.l   a0,d5
    addq    #1,d5
    ble.s   .Done

.FillRight:
    SAVE_CHAR   <#' '>

    subq    #1,d5
    bne.s   .FillRight

.Done:
    rts

; ################################################
; D0: Number
; D1: Outbuffersize
; D2: Base
; A0: Pointer to outputbuffer
; A1: Pointer to converter function
insertUnsignedNumber:
    movem.l a2-a3,-(sp)

    btst    #STR_PARAM_32,d6
    beq.s   .Word
    
    jsr     uLongToString
    bra.s   .CheckLower

.Word:
    jsr     uWordToString

.CheckLower:
    btst    #STR_LOWERCASE,d6
    beq.s   StoreString

    move.l  d0,d1

.Loop:
    move.b  (a0)+,d2
    cmp.b   #'A',d2
    blt.s   .Skip

    or.b    #$60,d2
    subq    #1,a0
    move.b  d2,(a0)+

.Skip:
    subq    #1,d1
    bne.s   .Loop

    sub.l   d0,a0

StoreString:
    move.b  #0,(a0)
    movem.l (sp)+,a2-a3

    sub.l   d0,a0

    jmp     insertString

; ################################################
; D0: Number
; D1: Outbuffersize
; D2: Base
; A0: Pointer to outputbuffer
; A1: Pointer to converter function
insertSignedNumber:
    movem.l a2-a3,-(sp)

    btst    #STR_PARAM_32,d6
    beq.s   .Word

    jsr     sLongToString
    bra.s   StoreString

.Word:
    jsr     sWordToString
    bra.s   StoreString

	section .data,data

_NullString::
NullString:: dc.b '[null]',0
