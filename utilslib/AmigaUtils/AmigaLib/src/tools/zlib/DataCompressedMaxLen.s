
	section .text,code

; #####################################################
;
; Return the maximum size the compressed buffer can
; reach. This can be larger then the input buffer if
; the source data is already compressed and can not be
; compressed more.
;
; INPUT:
;	D0:	Length of input buffer.
;
; RETURN:
;	D0:	Maximum lenght the outputbuffer can reach.
;
; Modified: D0,D1
;
; #####################################################

_zlibMaxCompressedLen::
zlibMaxCompressedLen::
	movem.l	d2,-(sp)

	move.l	d0,d2		; Length

	; uint32_t n16kBlocks = (nLenSrc+16383) / 16384;

	move.l	#16383,d1
	add.l	d1,d0
	addq	#1,d1
	jsr		div32u

    ; return (nLenSrc + 6 + (n16kBlocks * 5));

	move.l	d0,d1
	lsl.l	#2,d0
	add.l	d1,d0
	add.l	d2,d0
	addq	#6,d0

	movem.l	(sp)+,d2
	rts
