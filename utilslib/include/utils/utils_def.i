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
	IFGT NARG-10
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

; ******************************************************
; Sets the specified symbol to the MAX value of 
; param 2 and 3. The macro can be used multiple times
; with the same symbol name overwriting the current
; value with the new max value.
;
; PARAMS:
;	\1 SYMBOL: name of the symbol which is set.
;	\2 Value1
;	\3 Value2
;
; USAGE: SET_MAX Symbol, value1, value2
; EXAMPLE: SET_MAX UPPER_LIMIT,5,10
;	defines the symbol
;	UPPER_LIMIT SET 10
; ******************************************************
SET_MAX MACRO

	IF \2>\3
\1 SET \2
	ELSE
\1 SET \3
	ENDIF

	ENDM

	ENDIF
