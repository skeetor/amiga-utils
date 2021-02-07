	IFND _UTILS_DEF_I
_UTILS_DEF_I EQU 1

; CALL_LIB for calling functions with A6 is already loaded

; ******************************************************
; Call a library function with A6 pointing to the
; address table
; ******************************************************
CALL	MACRO   ; functionOffset
	IFGT NARG-1
		FAIL [ERR] CALL MACRO - too many arguments !!!
	ENDC
	JSR     \1(A6)

    ENDM

; ******************************************************
; Call a library function with A6 pointing to the
; system library base.
; ******************************************************
CALLLIB	MACRO   ; functionOffset
	IFGT NARG-1
		FAIL [ERR] CALLLIB MACRO - too many arguments !!!
	ENDC
	CALL	_LVO\1

    ENDM

; ******************************************************
; Jump to the library function with A6 containing the
; address table
; ******************************************************
GOTO	MACRO   ; functionOffset
	IFGT NARG-1
		FAIL [ERR] GOTO MACRO - too many arguments !!!
	ENDC
	JMP     \1(A6)

    ENDM


; ******************************************************
; Jump to the library function instead of jsr
; ******************************************************
JMPLIB	MACRO   ; functionOffset
	IFGT NARG-1
		FAIL [ERR] JMPLIB MACRO - too many arguments !!!
	ENDC
	GOTO     _LVO\1

    ENDM


; ******************************************************
; Allocate space on the stack and align the stack. For
; the alignment a Dn register is needed.
;
; Arguments:
;	SIZE  size to be allocated
;	ALIGNMENT Number to align to
;	REGISTER Register which is used for the aligment.
;			 This register will contain the original
;			 stackpointer, before the allocation.
;
; Result:
;	Dn contains the stackpointer before the allocation
;	and must be restored to when the stack is cleaned up.
;
;	A7 is decreased and points to the aligned block which
;	contains at least SIZE bytes.
;
; Example:
;	STACK_ALLOC		25,4,d0 will reserve at least 25
;		bytes on the stack and align it to 4, so it can
;		be used for a DOS library call.
;
; Note: Due to the alignment, the size does not have to be
; an even number, only the alignment must be even.
; If multiple structures are allocated in a row, obviously
; the first returned stackpointer is the original one.
;
; ******************************************************

STACK_ALLOC MACRO

SA_SIZE     set \+
SA_ALIGN    set \+

    IFGT SA_ALIGN-255
        FAIL [ERR] Alignment must be < 255
		MEXIT
    ENDC

SA_MODULO set SA_SIZE-((SA_SIZE/SA_ALIGN)*SA_ALIGN)

    move.l      a7,\3
    sub.l       #SA_SIZE+(SA_ALIGN-SA_MODULO),\3
    and.b       #$100-SA_ALIGN,\3
    exg      	\3,a7

    ENDM

MACRO_NOP	MACRO
	ENDM

ISREGISTER MACRO
REGISTER SET 0
REGNUM SET 0

	REPT 8
		IFC \1,d\<REGNUM>
REGISTER SET 1
		endif

		IFC \1,D\<REGNUM>
REGISTER SET 1
		endif

		IFC \1,a\<REGNUM>
REGISTER SET 1
		endif

		IFC \1,A\<REGNUM>
REGISTER SET 1
		endif

REGNUM SET \<REGNUM>+1
	ENDR

	ENDM

STDCALL	MACRO

CARG SET NARG
STACKPTR SET 0

; Push arguments in reverse order
REPCOUNT SET \<CARG>-1

	REPT NARG-1
		ISREGISTER \.
		IF REGISTER
			movem.l	\.,-(sp)
		ELSE
			pea		\.
		ENDIF

		MACRO_NOP \-

STACKPTR SET \<STACKPTR>+4

	ENDR

	jsr		_\1
	add.l	#\<STACKPTR>,sp

	ENDM

; ******************************************************
; Create constants for bitflags with a bitnumber and its
; corresponding mask.
;
; Arguments:
;	PREFIX		Prefix for the resulting constant
;	NAME		The name of the flag
;	BITNUMBER 	Number of the bit
;
; Result:
;	The name for the bitnumber and it's bitmask.
;
; Example:
;	BitFlag	BLT,COMPRESSION,2
;	Constants created:
;   BLT_BT_COMPRESSION	EQU 2		; Bitnumber
;   BLT_FL_COMPRESSION	EQU (1<<2)	; Bitmask
;
; ******************************************************
BitFlag MACRO

\1_BT_\2	EQU \3
\1_FL_\2	EQU (1<<\3)

	ENDM
	
	ENDIF
