	IFND _UTILS_COPPER_I
_UTILS_COPPER_I SET 1

; PARAM:
;	1 = Register
;	2 = Value

COP_MOVE MACRO

	IF (\#) != 2
	    FAIL [ERR] COPPER_MOVE - Invalid  number of arguments !!! Expected 2, given \#
	ENDC 

	IF \1 <= $3F
	    FAIL [ERR] COPPER_MOVE - Register can not be lower then $0040: Register \1
	ENDC
	
	IF \1 > $1FE
	    FAIL [ERR] COPPER_MOVE - Register can not be higher then $01FE: Register \1
	ENDC
	
	IF \1 & 1
	    FAIL [ERR] COPPER_MOVE - MOVE can only move to an even register address: Register \1
	ENDC

	dc.w \1, \2

	ENDM

COP_END MACRO

	dc.w $ffff, $fffe

	ENDM

	ENDIF
