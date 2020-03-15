
	include utils/utils_def.i
	include utils/tools/zlib/zlib.i

	section .text,code

; #####################################################
; Compress a buffer with the zlib compression.
;
; INPUT:
;	A0: Sourcebuffer
;	D0: Size of sourcebuffer
;	A1: Targetbuffer
;	D1: Size of Targetbuffer
;
; RETURN:
;	D0: Compressed buffer length
;
; Modified: D0,D1,A0,A1
;
; #####################################################

; z_stream zInfo = {0};

; zInfo.total_in =  zInfo.avail_in=  nLenSrc;
; zInfo.total_out = zInfo.avail_out= nLenDst;
; zInfo.next_in = (Bytef *)abSrc;
; zInfo.next_out = (Bytef *)abDst;
; uint32_t nErr, nRet = -1;
;| nErr = deflateInit(&zInfo, Z_DEFAULT_COMPRESSION);
;| nErr = inflateInit(&zInfo);
; if (nErr == Z_OK)
; {
;| 	nErr = deflate(&zInfo, Z_FINISH);
;| 	nErr = inflate(&zInfo, Z_FINISH);
; 	if (nErr == Z_STREAM_END)
; 		nRet = zInfo.total_out;
; }
;| deflateEnd(&zInfo);
;| inflateEnd(&zInfo);
; return nRet;

_zlibCompressData::
zlibCompressData::
	movem.l	d2/a2-a6,-(sp)

	lea		zlibDeflateInit,a3
	lea		zlibDeflate,a4
	lea		zlibDeflateEnd,a5

	bra.s	zlibCompression

_zlibUncompressData::
zlibUncompressData::
	movem.l	d2/a2-a6,-(sp)

	lea		zlibInflateInit,a3
	lea		zlibInflate,a4
	lea		zlibInflateEnd,a5

zlibCompression:

	move.l	a7,a6
	sub.l	#z_stream,a7

	move.l	a7,a2

	; Init input/output buffer
	move.l	d0,zstr_total_in(a2)
	move.l	d0,zstr_avail_in(a2)
	move.l	a0,zstr_next_in(a2)

	move.l	d1,zstr_total_out(a2)
	move.l	d1,zstr_avail_out(a2)
	move.l	a1,zstr_next_out(a2)

	; Clear rest of stream data
	moveq	#0,d0
	move.l	d0,zstr_msg(a2)
	move.l	d0,zstr_state(a2)
	move.l	d0,zstr_zalloc(a2)
	move.l	d0,zstr_zfree(a2)
	move.l	d0,zstr_opaque(a2)
	move.l	d0,zstr_data_type(a2)
	move.l	d0,zstr_adler(a2)
	move.l	d0,zstr_reserved(a2)

	jsr		(a3)
	tst.l	d0
	bne.s	.Error

	jsr		(a4)
	subq	#1,d0
	bne.s	.Error

	move.l	zstr_total_out(a2),d0

.zlibEnd:
	move.l	d0,d2
	jsr		(a5)
	move.l	d2,d0

	move.l	a6,a7
	movem.l	(sp)+,d2/a2-a6

	rts

.Error:
	moveq	#-1,d0
	bra.s	.zlibEnd

; ***** Compression
zlibDeflateInit:
	STDCALL deflateInit_,a2,Z_DEFAULT_COMPRESSION,ZLIB_VERSION,z_stream
	rts

zlibDeflate:
	STDCALL deflate,a2,Z_FINISH
	rts

zlibDeflateEnd:
	STDCALL	deflateEnd,a2
	rts

; ***** Decompression
zlibInflateInit:
	STDCALL inflateInit_,a2,ZLIB_VERSION,z_stream
	rts

zlibInflate:
	STDCALL inflate,a2,Z_FINISH
	rts

zlibInflateEnd:
	STDCALL	inflateEnd,a2
	rts
