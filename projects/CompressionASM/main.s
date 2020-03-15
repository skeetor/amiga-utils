
	include utils/utils_def.i
	include utils/system/libraries.i

	section .text,code

_start:

	moveq	#0,d0				; Version
	jsr		OpenDOSLibrary

	move.l	#uCompDataSize,uLen

	move.l	sp,a4
	pea		ZLIB_VERSION
	lea		HeaderTxt,a0
	jsr		printFormatted
	move.l	a4,sp

	; Calculate CRC of the uncompressed input buffer ...
	STDCALL	crc32,0,uCompData,uCompDataSize
	move.l	d0,uCRC32

	move.l	sp,a4
	movem.l	d0,-(sp)
	lea		UncompressedTxt,a0
	jsr		printFormatted
	move.l	a4,sp

	; ... and compress it.
	move.l	uLen,d0
	lea		uCompData,a0
	move.l	d0,d1
	lea		zCompData,a1
	jsr		zlibCompressData
	move.l	d0,cLen
	moveq	#-1,d1
	cmp.l	d1,d0
	beq		.FailedCompress

	; Now calculate the CRC of the compressed buffer.
	STDCALL	crc32,0,zCompData,d0
	move.l	d0,cCRC32

	; Print status
	move.l	sp,a4
	movem.l	d0,-(sp)		; Compressed CRC
	move.l	cLen,-(sp)
	move.l	uLen,-(sp)
	lea		CompressBufferTxt,a0
	jsr		printFormatted
	move.l	a4,sp

	; Decompress the compressed buffer.
	move.l	cLen,d0
	lea		zCompData,a0
	move.l	uLen,d1
	lea		zuCompData,a1
	jsr		zlibUncompressData
	move.l	d0,cuLen
	moveq	#-1,d1
	cmp.l	d1,d0
	beq.s	.FailedCompress

	STDCALL	crc32,0,zuCompData,d0
	move.l	d0,cuCRC32

	; Print uncompression status
	move.l	sp,a4
	movem.l	d0,-(sp)		; Compressed CRC
	move.l	cLen,-(sp)
	move.l	cuLen,-(sp)
	lea		UncompressBufferTxt,a0
	jsr		printFormatted
	move.l	a4,sp

	move.l	uCRC32,d0
	cmp.l	cuCRC32,d0
	bne.s	.SelftestFailed

	move.l	uLen,d0
	cmp.l	cuLen,d0
	bne.s	.SelftestFailed
	
.Done:
	jsr		CloseDOSLibrary

	moveq	#0,d0			;return 0 to the system
	rts

.FailedCompress:
	lea		UncompressedTxt,a0
	jsr		printFormatted
	bra.s	.Done

.SelftestFailed:
	move.l	a7,a5

	move.l	cuCRC32,-(sp)
	move.l	cuLen,-(sp)
	move.l	uCRC32,-(sp)
	move.l	uLen,-(sp)

	lea		SelftestFailedTxt,a0
	jsr		printFormatted
	move.l	a5,a7
	bra.s	.Done

; ********************************
; ************* DATA *************
; ********************************

	section .data,data

	CNOP 0,2
uCompData: dc.l 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
uCompDataSize = *-uCompData
zCompData: dc.l 0,0,0,0,0,0,0,0,0,0, 0, 0, 0, 0, 0, 0
zuCompData: dc.l 0,0,0,0,0,0,0,0,0,0, 0, 0, 0, 0, 0, 0
	CNOP 0,2
HeaderTxt: dc.b 'Testing zLib %s compression library\r\n\r\n',0
	CNOP 0,2
UncompressedTxt: dc.b 'Uncompressed CRC32: $%08lX\r\n',0
	CNOP 0,2
CompressBufferTxt: dc.b 'Compressed buffer from %lu to %lu bytes. CRC32: $%08lX\r\n',0
	CNOP 0,2
UncompressBufferTxt: dc.b 'Uncompressed buffer from %lu to %lu bytes. CRC32: $%08lX\r\n',0
	CNOP 0,2
FailedCompressTxt: dc.b 'Failed to de-/compress buffer!\r\n',0
	CNOP 0,2
SelftestFailedTxt: dc.b '[ERROR] Selftest failed %ld/$%08lX vs. %ld/$%08lX\r\n',0

	section .bss,bss

	CNOP 0,2

uCRC32:	ds.l 1
cCRC32:	ds.l 1
cuCRC32:	ds.l 1
uLen:	ds.l 1
cLen:	ds.l 1
cuLen:	ds.l 1
