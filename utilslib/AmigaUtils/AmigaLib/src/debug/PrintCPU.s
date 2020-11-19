; #####################################################
; Print all CPU registers
;
; INPUT:  -
; RETURN: -
;
; TRASHED: -
;
; #####################################################

	section .text,code

PrintCPU::
_PrintCPU::
    move.l a7,RegSP
    
    move.l	(a7),-(sp)
    move.l	RegSP,-(sp)
    movem.l d0-d7/a0-a6,-(sp)

	move.l	#bufferSize,d0
	lea		buffer,a0
	lea		CPUStr,a1
	move.l	a7,d1
	jsr		formatString
	lea		buffer,a0
	jsr		printString

    move.l	RegSP,a7
	sub.l	#17*4,a7
    movem.l (sp)+,d0-d7/a0-a6
    move.l	RegSP,a7

    rts

	section .data,data

	CNOP 0,2

RegSP:		ds.l 1

CPUStr: dc.b 'D0: %08lx D1: %08lx D2: %08lx D3: %08lx\n\r'
		dc.b 'D4: %08lx D5: %08lx D6: %08lx D7: %08lx\n\r'
		dc.b 'A0: %08lx A1: %08lx A2: %08lx A3: %08lx\n\r'
		dc.b 'A4: %08lx A5: %08lx A6: %08lx A7: %08lx\n\r'
		dc.b 'PC: %08lx\n\r',0

	CNOP 0,2
buffer:	ds.b 255
bufferSize = *-buffer
