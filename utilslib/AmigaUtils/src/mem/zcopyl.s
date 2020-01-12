	section .text,code

; Copy 32 bit values until a 0-terminator is encountered.
; The number of copied items (including the 0-terminator)
; is returned.

_zcopyl::
zcopyl::
    moveq #-1,d0

.Loop:
    move.l  (a0)+,(a1)+
    dbeq    d0,.Loop

	neg.l d0

    rts
