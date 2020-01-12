	IFND _UTILS_VALIDATE_I
_UTILS_VALIDATE_I EQU 1

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

; ******************************************************
; Validation macros:
;
; VALIDATE_GT: Error if Param1 <= Param2
; VALIDATE_LT: Error if Param1 >= Param2
; VALIDATE_GE: Error if Param1 < Param2
; VALIDATE_LE: Error if Param1 > Param2
; VALIDATE_EQ: Error if Param1 != Param2
; VALIDATE_NE: Error if Param1 == Param2
;
; ******************************************************

VALIDATE_GT MACRO
PARAM1 set \1
PARAM2 set \2

	if \1<=\2
		FAIL [ERR] \1 (\<PARAM1>) <= \2 (\<PARAM2>)
	endif

	ENDM

VALIDATE_LT MACRO
PARAM1 set \1
PARAM2 set \2

	if \1>=\2
		FAIL [ERR] \1 (\<PARAM1>) >= \2 (\<PARAM2>)
	endif

	ENDM

VALIDATE_GE MACRO
PARAM1 set \1
PARAM2 set \2

	if \1<\2
		FAIL [ERR] \1 (\<PARAM1>) < \2 (\<PARAM2>)
	endif

	ENDM

VALIDATE_LE MACRO
PARAM1 set \1
PARAM2 set \2

	if \1>\2
		FAIL [ERR] \1 (\<PARAM1>) > \2 (\<PARAM2>)
	endif

	ENDM

VALIDATE_EQ MACRO
PARAM1 set \1
PARAM2 set \2

	if \1!=\2
		FAIL [ERR] \1 (\<PARAM1>) != \2 (\<PARAM2>)
	endif

	ENDM

VALIDATE_NE MACRO
PARAM1 set \1
PARAM2 set \2

	if \1==\2
		FAIL [ERR] \1 (\<PARAM1>) == \2 (\<PARAM2>)
	endif

	ENDM

	ENDIF
