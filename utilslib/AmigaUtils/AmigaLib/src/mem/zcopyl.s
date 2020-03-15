	section .text,code

; Copy 32 bit values until a 0-terminator is encountered (which is also copied).
; The number of copied items (excluding the 0-terminator)
; is returned.

_zcopyl::
zcopyl::
    moveq #-1,d0

.Loop:
    move.l  (a0)+,(a1)+
    dbeq    d0,.Loop

	neg.l	d0
	sub.l	#1,d0

    rts
