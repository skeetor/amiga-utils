	IFND _UTILS_GFXBASE_I
_UTILS_GFXBASE_I EQU 1

; Values for GfxBase->DisplayFlags
DFL_NTSC		EQU 1
DFL_GENLOC		EQU 2
DFL_PAL			EQU 4
DFL_TODA_SAFE	EQU 8
DFL_REALLY_PAL	EQU 16	; What is actual crystal frequency
						; (as opposed to what bootmenu set the agnus to)?
						; (V39)


	ENDIF
