
	include lvo/dos_lib.i
	include utils/system/libraries.i

	include utils/hardware/custom_registers.i

	section .text,code

_EnableDiskIO::
EnableDiskIO::
	; http://eab.abime.net/showthread.php?p=1367099#post1367099
	move.w	#INT_SET|INT_PORTS,INTENA
	move.w	#DMA_SET|DMA_BLTEN|DMA_DSKEN,DMACON
	rts

DOSInit:
	move.l	DOSBase,a6
	CALLLIB	Output
	move.l	d0,stdout

	rts

DOSShutdown:
	rts

	CreateSystemLibrary DOS

; *******************************************************************************
; *******************************************************************************
; DATA
; *******************************************************************************
; *******************************************************************************

	section .data,data

	CNOP 0,2

stdout::	dc.l 1
DOSName::	dc.b	'dos.library',0
