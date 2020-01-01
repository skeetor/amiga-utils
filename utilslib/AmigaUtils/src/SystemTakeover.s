; Based on: http://stingray.untergrund.net/MiniStartup.s

	include exec/execbase.i

	include lvo/exec_lib.i
	include lvo/graphics_lib.i

	include utils/utils_def.i
	include utils/hardware/custom_registers.i

INTENASET	= %1100000000100000
DMASET		= %1000001111100000

	section .text,code

_SystemSave::
SystemSave::

	movem.l	d0-a6,-(a7)

	move.w	#0,NTSC_SYSTEM
	move.l	4.w,a6
	cmp.b	#50,PowerSupplyFrequency(a6)
	beq		.pal
	move.w	#1,NTSC_SYSTEM

.pal:
	moveq	#0,d0				; Version
	jsr		OpenGraphicsLibrary
	beq		.END

	move.l	d0,a6
	move.l	34(a6),OldView

	sub.l	a1,a1
	bsr		DoView

	move.l	DSKDAT_OFS(a6),OldCop1			; Store old CL 1
	move.l	SERPER_OFS(a6),OldCop2			; Store old CL 2
	
	bsr	GetVBR

	move.l	d0,VBRptr
	move.l	d0,a0

	;***	Store Custom Regs	***

	lea	HWREGBASE,a6						; base address
	move.w	ADKCONR_OFS(a6),ADK				; Store old ADKCON
	move.w	INTENAR_OFS(a6),OldINTENA		; Store old INTENA
	move.w	DMACONR_OFS(a6),DMAVal			; Store old DMA
	move.w	#$7FFF,d0

	jsr	WaitRaster

	move.w	d0,INTENA_OFS(a6)				; Disable Interrupts
	move.w	d0,DMACON_OFS(a6)				; Clear all DMA channels
	move.w	d0,INTREQ_OFS(a6)				; Clear all INT requests

	move.l	$6c(a0),OldVBI
	lea	NewVBI,a1
	move.l	a1,$6c(a0)

	move.w	#INTENASET!$C000,INTENA_OFS(a6)	; set Interrupts+ BIT 14/15
	move.w	#DMASET!$8200,DMACON_OFS(a6)	; set DMA	+ BIT 09/15
	move.w  #%0000111100000000,POTGO_OFS(a6); Set standard OS value
	move.w	#0,BPLCON0_OFS(a6)

.END:

	movem.l	(a7)+,d0-a6

    rts

_SystemRestore::
SystemRestore::

	movem.l	d2-d7/a2-a6,-(a7)

	lea	HWREGBASE,a6
	clr.l	VBIptr

	move.w	#$8000,d0
	or.w	d0,OldINTENA				; SET/CLR-Bit to 1
	or.w	d0,DMAVal					; SET/CLR-Bit to 1
	or.w	d0,ADK						; SET/CLR-Bit to 1
	subq.w	#1,d0
	jsr	WaitRaster

	move.w	d0,INTENA_OFS(a6)			; Clear all INT bits
	move.w	d0,DMACON_OFS(a6)			; Clear all DMA channels
	move.w	d0,INTREQ_OFS(a6)			; Clear all INT requests

	move.l	VBRptr,a0
	move.l	OldVBI,$6c(a0)

	move.l	OldCop1,COP1LCH_OFS(a6)		; Restore old CL 1
	move.l	OldCop2,COP2LCH_OFS(a6)		; Restore old CL 2
	move.w	d0,COPJMP1_OFS(a6)			; start copper1
	move.w	OldINTENA,INTENA_OFS(a6)	; Restore INTENA
	move.w	DMAVal,DMACON_OFS(a6)		; Restore DMAcon
	move.w	ADK,ADKCON_OFS(a6)			; Restore ADKcon

	move.l	GraphicsBase,a6
	move.l	OldView,a1					; restore old viewport
	bsr.b	DoView

	jsr		CloseGraphicsLibrary
	movem.l	(a7)+,d2-d7/a2-a6

	rts

DoView:
	; Two WaitTOF() calls are needed after the LoadView to wait for both the
	; long and short frame copperlists of interlaced displays to finish.

	CALLLIB LoadView
	CALLLIB WaitTOF
	JMPLIB WaitTOF

*******************************************
*** Get Address of the VBR		***
*******************************************

GetVBR:
	move.l	a5,-(a7)
	moveq	#0,d0				; default at $0
	move.l	ExecBase,a6
	btst	#0,296+1(a6)		; 68010+?
	beq.b	.is68k				; nope.
	lea		.getit(pc),a5
	CALLLIB Supervisor

.is68k:
	move.l	(a7)+,a5
	rts

.getit:

	mc68010
	movec   vbr,d0
	mc68000

	rte				; back to user state code

*******************************************
*** VERTICAL BLANK (VBI)		        ***
*******************************************

_ClearVBI::
ClearVBI::
	moveq	#0,d0
	move.l	d0,a0

_SetVBI::
SetVBI::
	move.l	VBIptr,d0
	move.l	a0,VBIptr
	rts

NewVBI:
	movem.l	d0-a6,-(a7)
	move.l	VBIptr,d0
	beq.b	.noVBI
	move.l	d0,a0
	jsr	(a0)
	
.noVBI:
	lea	$dff09c,a6
	moveq	#$20,d0
	move.w	d0,(a6)
	move.w	d0,(a6)			; twice to avoid a4k hw bug
	movem.l	(a7)+,d0-a6
	rte

; *******************************************************************************
; *******************************************************************************
; DATA
; *******************************************************************************
; *******************************************************************************

	section .bss,bss

	CNOP 0,2
OldView:	ds.l 1
NTSC_SYSTEM::	ds.w 1

OldCop1:	ds.l 1
OldCop2:	ds.l 1
VBRptr:		ds.l 1
OldVBI:		ds.l 1
ADK:		ds.w 1
OldINTENA:	ds.w 1
DMAVal:		ds.w 1
VBIptr:		ds.l 1
