
	section .text,code

; Faster replacement for the gcc modulo32 operation
___umodsi3::
    movem.l     4(sp),d0-d1
	bsr.s		div32u
	move.l		d1,d0	
	rts

; Faster replacement for the gcc div32 operation
___udivsi3::
    movem.l     4(sp),d0-d1
	jmp			div32u
