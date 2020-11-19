
	section .text,code

rndInit::
_rndInit::
	lea $dff000,a0    ; custom chips
	lea $dc0000,a1    ; clock (if there is one)
	moveq #15,d7
	move.w (a0),d0
	swap d0
	move.w d1,d0      ; d1 is dos bptr upon program entry

.loop:
	rol.l #5,d0
	add.w (a1),d0
	addq.l #4,a3
	dbf d7,.loop

	add.l $a(a0),d0
	rol.l #7,d0
	add.l $12(a0),d0

	jsr		rndSetSeed
	rts

rndSetSeed::
_rndSetSeed::
	move.l	d0,rndSeed
	move.l	d0,rndStartSeed
	rts

rndGetSeed::
_rndGetSeed::
	move.l	rndStartSeed,d0
	rts

; #####################################################
; Create a pseudo random 32 bit number using
; XORShift algorithm:
; http://eab.abime.net/showthread.php?t=104687
;
; INPUT:
;
; RETURN:
; 	d0: 32 bit pseudo random number
;
; TRASHED: d1, a0
;
; #####################################################

random::
_random::
    lea     rndSeed(pc),a0
    move.l  (a0),d0
	move.l  4(a0),d1
	swap	d1
	add.l	d0,d1
	eor.l	d0,d1
	move.l  d0,4(a0)
	move.l  d1,(a0)
	move.l  d1,d0
	rts

rndStartSeed: dc.l  $cd2ef3b7    
rndSeed: dc.l  $cd2ef3b7    
.rndValue: dc.l $6be7d35b
