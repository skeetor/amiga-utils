
	section .text,code

; #####################################################
; Multiply signed/unsigned 32 bit value by signed/unsigned32
; bit value.
;
; INPUT:
;	d0: Num1
;	d1: Num2
;
; RETURN:
; 	d0: Result
;
; MODIFIED: d1
;
; #####################################################

___mulsi3::
    movem.l     4(sp),d0-d1
	jmp			mult32u
