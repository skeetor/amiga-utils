
	include lvo/dos_lib.i
	include utils/system/libraries.i

	section .text,code

_vPrintFormatted::
	move.l	stdout,d0

_vWriteFormatted::
	bra.s	wfmIntern

; #####################################################
; Write string to stdout. For details refer to 
; formatString.
;
; INPUT:
;	A0: Formatstring
;
; RETURN: See FormatString
;
; Modified: D0, D1, A0-A1
;
; #####################################################

PrintFormatted::
_PrintFormatted::
	move.l	stdout,d0

_WriteFormatted::
	lea		4(sp),a1
	move.l	a1,d1

wfmIntern:
	move.l	#128,a1

; #####################################################
; Write string to stdout/file. For details refer to 
; FormatString.
;
; INPUT:
;	D0: Outputfilehandle
;	D1:	Pointer to first parameter on stack
;	A0: Formatstring
;	A1: Default buffer length
;
; RETURN: See formatString
;
; Modified: D0, D1, A0-A1
;
; #####################################################
WriteFormatted::
	movem.l	d2-d3/a5-a6,-(sp)
	movem.l	d0,-(sp)		; save Filehandle
	move.l	a1,d0

.Retry:
	movem.l	d1/a0,-(sp)		; save Formatstring and Parampointer
	move.l	a7,a5

	; align stack to long boundary
	move.l	a7,d2
	addq	#3,d0
	sub.l	d0,d2
	and.b	#$fc,d2
	move.l	a5,d0
	sub.l	d2,d0
	move.l	d2,a7
	movem.l	d0,-(sp)

	move.l	a0,a1
	move.l	d2,a0
	move.l	d2,a6
	jsr		FormatString

	move.l	d0,d2
	movem.l	(sp)+,d0
	move.l	a5,a7

	movem.l (sp)+,d1/a0		; Restore Parampointer and string
	exg		d2,d0
	cmp.l	d0,d2
	blt.s	.Retry

	movem.l	(sp)+,d1		; Filehandle
	move.l	a6,d2			; Buffer
	move.l	d0,d3			; Length

	move.l	a7,a5			; Save stackpopinter
	move.l	a6,a7			; and set it to our prepared string
	move.l	DOSBase,a6
	CALLLIB	Write
	move.l	a5,a7

	movem.l	(sp)+,d2-d3/a5-a6
	rts
