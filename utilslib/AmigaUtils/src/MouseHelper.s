	include utils/hardware/custom_registers.i

; Check if the left or right Mousebutton has been pressed. If yes,
; it waits untilt it is released again. If not, the function returns
; immediately with Z-flag cleared.
; The Zeroflag is set accordingly. Clear if not button was pressed
; and set if the button was pressed and has been released.

	section .text,code

_WaitLeftMouseRelease::
WaitLeftMouseRelease::
	
	btst	#6,CIAAPRA				; Left button
	bne		.None

.WaitRelease:
	btst	#6,CIAAPRA	
	beq		.WaitRelease

.Done:
	moveq	#1,d0
	rts

.None:
	moveq #0,d0
	rts
	
_WaitRightMouseRelease::
	moveq	#0,d0
	jsr		WaitRightMouseRelease
	beq		.Done
	moveq	#1,d0
	
.Done:
	rts

WaitRightMouseRelease::

	btst	#2,POTGOR				; Right button
	bne		.Done

.WaitRelease:
	btst	#2,POTGOR				; Right button
	beq		.WaitRelease

.Done:
	moveq	#1,d0
	rts

.None:
	moveq #0,d0
	rts
