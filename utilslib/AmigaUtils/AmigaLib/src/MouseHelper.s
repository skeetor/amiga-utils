	include utils/hardware/custom_registers.i

; Check if the left or right Mousebutton has been pressed. If yes,
; it waits until it is released again. If not, the function returns
; immediately with Z-flag cleared.
; The Zeroflag is set accordingly. Clear if not button was pressed
; and set if the button was pressed and has been released.

	section .text,code

_CheckLeftMouseRelease::
CheckLeftMouseRelease::
	
	btst	#6,CIAAPRA				; Left button
	bne		.None

.WaitRelease:
	btst	#6,CIAAPRA	
	beq		.WaitRelease

	moveq	#1,d0
	rts

.None:
	moveq #0,d0
	rts
	
_CheckRightMouseRelease::
CheckRightMouseRelease::

	btst	#2,POTGOR				; Right button
	bne		.None

.WaitRelease:
	btst	#2,POTGOR				; Right button
	beq		.WaitRelease

	moveq	#1,d0
	rts

.None:
	moveq #0,d0
	rts
