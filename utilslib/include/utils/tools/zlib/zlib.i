	IFND _ZLIB_I
_ZLIB_I EQU 1

	rsreset
zstr_next_in:			rs.l 1
zstr_avail_in:			rs.l 1
zstr_total_in:			rs.l 1
zstr_next_out:			rs.l 1
zstr_avail_out:			rs.l 1
zstr_total_out:			rs.l 1
zstr_msg:				rs.l 1
zstr_state:				rs.l 1
zstr_zalloc:			rs.l 1
zstr_zfree:				rs.l 1
zstr_opaque:			rs.l 1
zstr_data_type:			rs.l 1
zstr_adler:				rs.l 1
zstr_reserved:			rs.l 1
z_stream:				rs.b 0


; Allowed flush values; see deflate() and inflate() below for details
Z_NO_FLUSH			EQU 0
Z_PARTIAL_FLUSH		EQU 1
Z_SYNC_FLUSH		EQU 2
Z_FULL_FLUSH		EQU 3
Z_FINISH			EQU 4
Z_BLOCK				EQU 5
Z_TREES				EQU 6


; Return codes for the compression/decompression functions. Negative values
; are errors, positive values are used for special but normal events.
Z_OK				EQU 0
Z_STREAM_END		EQU 1
Z_NEED_DICT			EQU 2
Z_ERRNO				EQU -1
Z_STREAM_ERROR		EQU -2
Z_DATA_ERROR		EQU -3
Z_MEM_ERROR			EQU -4
Z_BUF_ERROR			EQU -5
Z_VERSION_ERROR		EQU -6

; compression levels
Z_NO_COMPRESSION	EQU 0
Z_BEST_SPEED		EQU 1
Z_BEST_COMPRESSION	EQU 9
Z_DEFAULT_COMPRESSION	EQU -1

; compression strategy; see deflateInit2() below for details
Z_FILTERED			EQU 1
Z_HUFFMAN_ONLY		EQU 2
Z_RLE				EQU 3
Z_FIXED				EQU 4
Z_DEFAULT_STRATEGY	EQU 0

; Possible values of the data_type field for deflate()
Z_BINARY			EQU 0
Z_TEXT				EQU 1
Z_ASCII				EQU Z_TEXT   ; for compatibility with 1.2.2 and earlier
Z_UNKNOWN			EQU 2

; The deflate compression method (the only one supported in this version)
Z_DEFLATED			EQU 8

; for initializing zalloc, zfree, opaque
Z_NULL				EQU 0

	ENDIF
