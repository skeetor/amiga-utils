	include exec/types.i

	include utils/system/libraries.i
	include utils/graphics/gfxbase.i

	section .text,code

_start:

	moveq	#0,d0				; Version
	jsr		OpenDOSLibrary

	move.l	#786575,d0
	move.l	#17,d1
	jsr		mult32u
	add.l	#414800,d0
	; 13786575 / 786575 = 17 Mod: 414800

	move.l	#786575,d1
	jsr		PrintDivision

	jsr		CloseDOSLibrary

	moveq	#0,d0			;return 0 to the system
	rts

PrintDivision:
	move.l	d0,d6
	move.l	d1,d7

	; 13786575 / 786575
	jsr		div32u

	move.l	a7,a2

	move.l	d1,-(sp)		; Modulo: 414800
	pea		Remainder
	move.l	d0,-(sp)		; Result 17
	move.l	d7,-(sp)		; 786575
	move.l	d6,-(sp)		; 13786575
	lea		outbuffer,a0
	lea		OutString,a1
	move.l	#OutbufferSize,d0
	move.l	a7,d1
	jsr		formatString
	move.l	a2,a7

	lea		outbuffer,a0
	jsr		puts
	
	rts


; ********************************
; ************* DATA *************
; ********************************

	section .data,data

	CNOP 0,2

Remainder: dc.b 'Remainder: ',0
OutString: dc.b '%lu / %lu = %lu %s: %lu\n\r',0

	section .bss,bss

	CNOP 0,2

outbuffer:	ds.b 512
OutbufferSize = *-outbuffer
