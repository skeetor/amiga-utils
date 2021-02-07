	include exec/memory.i

	include utils/system/libraries.i
	include utils/hardware/custom_registers.i

	section .text,code

_start:
	jsr		SystemSave

	moveq	#0,d0				; Version
	jsr		OpenDOSLibrary

	; We have to enable disk IO so we can
	; use the dos.library functions for accessing
	; the filesystem.
	; This has to be called only once, or again if
	; the relevant DMA/IRQ settings are disabled.
	; If the appropriate DMA/IRQ settings are already
	; enabled manually, then this call is not needed.
	jsr		EnableDiskIO

	lea		MODName,a0
	move.l	#MEMF_CHIP,d0
	jsr		LoadFile
	
	move.l	d0,ModSize
	move.l	a0,ModMem
	move.l	a0,d0
	beq.s	.Quit

	jsr		PlayMOD

	move.l	ModSize,d0
	move.l	ModMem,a1
	move.l	ExecBase,a6
	CALLLIB	FreeMem

.Quit:
	; Print to stdout obviously only works after
	; being back in AmigaDOS
	jsr		SystemRestore

	; Parameters for printing the file info
	move.l	a7,a5			; Remember for stack cleanup
	move.l	ModMem,-(sp)
	move.l	ModSize,-(sp)
	pea		MODName

	move.l	#OutbufferSize,d0
	lea		outbuffer,a0
	lea		FileString,a1
	move.l	a7,d1
	jsr		FormatString
	move.l	a5,a7
	lea		outbuffer,a0
	jsr		PrintString

	jsr		CloseDOSLibrary

	moveq	#0,d0			;return 0 to the system
	rts

; #####################################################
;
; The sample MOD was created using OpenMPT on Windows.
; No conversion required, it plays out of the box. :)
;
; #####################################################

PlayMOD:
	lea		HWREGBASE,a6
	moveq	#0,d0
	move.l	d0,a0
	move.w	PAL_SYSTEM,d0
	jsr		_mt_install_cia

	move.l	ModMem,a0	; MOD
	moveq	#0,d0		; Song position form start
	sub.l	a1,a1		; Samples (included in the MOD)
	jsr		_mt_init

	; Start playing
	move.b	#1,_mt_Enable

.WaitClick:
	jsr		CheckLeftMouseRelease
	beq		.WaitClick

	; Stop playing
	jsr		_mt_end

	; Deinitalize player
	jsr _mt_remove_cia

	rts

; ********************************
; ************* DATA *************
; ********************************

	section .data,data

	CNOP 0,2

MODName: dc.b 'DemoTrack.mod',0
FileString: dc.b 'Filesize: %s - %lu -%p\n\r',0

	section .bss,bss

	CNOP 0,2

ModSize:	ds.l 1
ModMem:		ds.l 1

outbuffer:	ds.b 512
OutbufferSize = *-outbuffer
