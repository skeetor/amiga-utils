	include lvo/exec_lib.i
	include lvo/graphics_lib.i

	include utils/system/libraries.i

	section .text,code
	
	CreateSystemLibrary Graphics

; *******************************************************************************
; *******************************************************************************
; DATA
; *******************************************************************************
; *******************************************************************************

	section .data,data

	CNOP 0,2

GraphicsName::	dc.b	'graphics.library',0
