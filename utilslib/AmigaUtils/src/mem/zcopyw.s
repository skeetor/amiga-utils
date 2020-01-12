	section .text,code

; Copy 16 bit values until a 0-terminator is encountered.
; The number of copied items (including the 0-terminator)
; is returned.

_zcopyw::
zcopyw::
    moveq #-1,d0

.Loop:
    move.w  (a0)+,(a1)+
    dbeq    d0,.Loop

	neg.l d0

    rts
