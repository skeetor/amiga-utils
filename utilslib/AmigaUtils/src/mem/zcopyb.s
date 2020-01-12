	section .text,code

; Copy 8 bit values until a 0-terminator is encountered.
; The number of copied items (including the 0-terminator)
; is returned.

_zcopyb::
zcopyb::
    moveq #-1,d0

.Loop:
    move.b  (a0)+,(a1)+
    dbeq    d0,.Loop

	neg.l d0

    rts
