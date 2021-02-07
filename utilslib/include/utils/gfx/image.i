	IFND _UTILS_IMAGE_I
_UTILS_IMAGE_I EQU 1

	include utils/utils_def.i

BIF_FL_NONE 		EQU 0

	BitFlag BIF,ZLIB,0				; Bitmap data is zlib compressed
	BitFlag BIF,INTERLEAVE,1		; Bitmap format is interleaved

; The bitplane is always rounded to 16 bits, so if the image width is i.e. 
; only 2 pixels, there are 14 pixels empty as still 16 bits stored per bitmap line.
	rsreset
BIF_Id:				rs.b 5		; Contains "ABMI\0"
BIF_Flags:			rs.b 1
BIF_Width:			rs.w 1
BIF_Height:			rs.w 1
BIF_Frames:			rs.w 1		; Number of consecutive images in this file. All
								; images have the same properties (width, height, planes, etc.)
BIF_Planes:			rs.b 1
BIF_Colors:			rs.b 1		; May be 0 if the image file contains no colors
BitmapImageFile:	rs.b 0


	rsreset
; These values must initialized by the user.

BMI_Width			rs.w	1
BMI_Height			rs.w	1
BMI_Planes			rs.w	1
BMI_Flags			rs.w	1	; BIF_FL_INTERLEAVE
BMI_Frames			rs.w	1
BMI_Bitplane		rs.l	1	; Pointer to bitplane

; These values need to be calculated before blitting by
; calling InitBitmapInfo. This has to be redone once
; after the above values have been changed.

BMI_BytesPerRow		rs.w	1
BMI_LineDistance	rs.l	1	; Number of bytes to the next line.
BMI_PlaneDistance	rs.l	1	; Number of bytes to same line of the next plane.
BMI_NextLine		rs.l	1	; Number of bytes to the next line after the end of a line
BMI_NextPlane		rs.l	1	; Number of bytes to next plane after the end of a line.

BitmapInfo			rs.b	0

	ENDIF
