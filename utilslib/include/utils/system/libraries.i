	IFND _UTILSLIB_LIBRARIES_I
_UTILSLIB_LIBRARIES_I SET 1

	include utils/utils_def.i
	include lvo/exec_lib.i

SystemBase		EQU $4
ExecBase		EQU $4

; CreateSystemLibrary creates the required functions and variables
; for a system library. It also provides a reference count, so the
; library is only opened once for multiple calls.
;
; <lib>Name must be defined by the user.
;
; Provided functions/variables:
; 	Open<lib>Library - Opens the specified library, D0 = Version
;	Close<lib>Library - Closes the library if the reference count reaches 0.
;	<lib>Base - The base of the library as returned by OpenLibrary. This value
;				is set to NULL when the last  user closes the library.
;
; PARAM:
;	1 = Symbolic of the library
;
; EXAMPLE:
; 	CreateSystemLibrary DOS

CreateSystemLibrary MACRO

_Open\1Library::
Open\1Library::

    movem.l a6,-(a7)

    move.l  \1Base,d1
    bne     .IncRef

	lea		\1Name,a1
	move.l	ExecBase,a6
	CALLLIB	OpenLibrary
    move.l  d0,\1Base
    move.l  d0,d1
    beq     .NoRef

	jsr		\1Init

.IncRef:
    add.w   #1,\1BaseRef

.NoRef:
	move.l	d1,d0
    movem.l (a7)+,a6

    rts

_Close\1Library::
Close\1Library::

	move.w	\1BaseRef,d0
	beq		.Done
	sub.w	#1,\1BaseRef

	move.l	\1Base,a1
	beq		.Done

	movem.l	a6,-(a7)

	jsr		\1Shutdown

	move.l	ExecBase,a6
	CALLLIB CloseLibrary
	clr.l	\1Base

	movem.l	(a7)+,a6

.Done:
	rts

	section .bss,bss

	CNOP 0,2

\1Base::	ds.l 1
\1BaseRef:	ds.l 1

	ENDM

	ENDIF
