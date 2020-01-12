	include exec/types.i

	include utils/system/libraries.i
	include utils/graphics/gfxbase.i

	section .text,code

_start:

	moveq	#0,d0				; Version
	jsr		OpenDOSLibrary

	move.l	#786575,d0
	move.l	#17,d1
	jsr		mul32u
	add.l	#414800,d0
	; 13786575 / 786575 = 17 Mod: 414800

	move.l	#786575,d1
	move.l	#OutbufferSize,d2
	lea		outbuffer,a0
	movem.l	d0-d2/a0,-(a7)
	jsr		PrintDivision
	move.l	d0,d3
	movem.l	(a7)+,d0-d2/a0

	jsr		CloseDOSLibrary

	moveq	#0,d0			;return 0 to the system
	rts

PrintDivision:
	move.l	d0,d2
	move.l	d1,d3
	move.l	d2,d4
	move.l	a0,a2
	move.l	a0,a3

	move.l	d2,d0				; Backup for later
	move.l	d3,d1
	jsr		div32u

    ; Remember result/remainder for later
    move.l  d0,d5
    move.l  d1,d6

    ; Create divdidend
	move.l	a2,a0
	move.l	d2,d0
	move.l	d4,d1
	jsr		toDecimalU32

    add.l   d0,a2
    sub.l   d0,d4

    move.w  #' '<<8+'/',(a2)+
    move.b  #' ',(a2)+
    sub.l   #3,d4

    ; Create divisor
	move.l	a2,a0
	move.l	d3,d0
	move.l	d4,d1
	jsr		toDecimalU32

    add.l   d0,a2
    sub.l   d0,d4

    move.b  #' ',(a2)+
    move.w  #'='<<8+' ',(a2)+
    sub.l   #3,d4

    ; add result
	move.l	a2,a0
	move.l	d5,d0
	move.l	d4,d1
	jsr		toDecimalU32
    add.l   d0,a2
    sub.l   d0,d4

    move.b  #' ',(a2)+
    sub.l   #1,d4

    lea     Remainder,a0
    move.l  a2,a1
    jsr     strcpy
    add.l   d0,a2
    sub.l   d0,d4

	move.l	a2,a0
	move.l	d6,d0
	move.l	d4,d1
	jsr		toDecimalU32
    add.l   d0,a2
    sub.l   d0,d4

    move.l  #$0d0a0000,(a2)
	
	move.l	a3,a0
	jsr		puts
	
    move.l  d6,d0
	rts


; ********************************
; ************* DATA *************
; ********************************

	section .data,data

	CNOP 0,2

Remainder: dc.b 'Remainder: ',0

	section .bss,bss

	CNOP 0,2

outbuffer:	ds.b 512
OutbufferSize = *-outbuffer
