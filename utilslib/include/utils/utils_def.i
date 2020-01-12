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
; Jump to the librar function instead of jsr
; ******************************************************
JMPLIB	MACRO   ; functionOffset
	IFGT NARG-1
		FAIL [ERR] JMPLIB MACRO - too many arguments !!!
	ENDC
	GOTO     _LVO\1

    ENDM

	ENDIF
