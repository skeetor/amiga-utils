	IFND _UTILS_CUSTOM_REGISTERS_I
_UTILS_CUSTOM_REGISTERS_I EQU 1

CIAAPRA		EQU $BFE001
HWREGBASE	EQU $DFF000

HWREG	MACRO

\1 EQU HWREGBASE+\2
\1_OFS EQU \2

    ENDM

	HWREG BLTDDAT,	$000		; Blitter destination early read (unusable)
	HWREG DMACONR,	$002		; DMA control (and blitter status) read
	HWREG VPOSR,	$004		; Read vertical raster position bit 9 (and interlace odd/even frame)
	HWREG VHPOSR,	$006		; Rest of raster XY position - High byte: vertical, low byte: horizontal
	HWREG DSKDATR,	$008		; Disk data early read (unusable)
	HWREG JOY0DAT,	$00a		; Joystick/mouse 0 data
	HWREG JOT1DAT,	$00c		; Joystick/mouse 1 data
	HWREG CLXDAT,	$00e		; Poll (read and clear) sprite collision state
	HWREG ADKCONR,	$010		; Audio, disk control register read
	HWREG POT0DAT,	$012		; Pot counter pair 0 data
	HWREG POT1DAT,	$014		; Pot counter pair 1 data
	HWREG POTGOR,	$016		; Pot pin data read
	HWREG SERDATR,	$018		; Serial port data and status read
	HWREG DSKBYTR,	$01a		; Disk data byte and status read
	HWREG INTENAR,	$01c		; Interrupt enable bits read
	HWREG INTREQR,	$01e		; Interrupt request bits read
	HWREG DSKPTH,	$020		; Disk track buffer pointer (high 5 bits)
	HWREG DSKPTL,	$022		; Disk track buffer pointer (low 15 bits)
	HWREG DSKLEN,	$024		; Disk track buffer length
	HWREG DSKDAT,	$026		; Disk DMA data write
	HWREG REFPTR,	$028		; AGA: Refresh pointer
	HWREG VPOSW,	$02a		; Write vert most sig. bits (and frame flop)
	HWREG VHPOSW,	$02c		; Write vert and horiz pos of beam
	HWREG COPCON,	$02e		; Coprocessor control register (CDANG)
	HWREG SERDAT,	$030		; Serial port data and stop bits write
	HWREG SERPER,	$032		; Serial port period and control
	HWREG POTGO,	$034		; Pot count start, pot pin drive enable data
	HWREG JOYTEST,	$036		; Write to all 4 joystick/mouse counters at once
	HWREG STREQU,	$038		; Strobe for horiz sync with VBLANK and EQU
	HWREG STRVBL,	$03a		; Strobe for horiz sync with VBLANK
	HWREG STRHOR,	$03c		; Strobe for horiz sync
	HWREG STRLONG,	$03e		; Strobe for identification of long/short horiz line
	HWREG BLTCON0,	$040		; Blitter control reg 0
	HWREG BLTCON1,	$042		; Blitter control reg 1
	HWREG BLTAFWM,	$044		; Blitter first word mask for source A
	HWREG BLTALWM,	$046		; Blitter last word mask for source A
	HWREG BLTCPTH,	$048		; Blitter pointer to source C (high 5 bits)
	HWREG BLTCPTL,	$04a		; Blitter pointer to source C (low 15 bits)
	HWREG BLTBPTH,	$04c		; Blitter pointer to source B (high 5 bits)
	HWREG BLTBPTL,	$04e		; Blitter pointer to source B (low 15 bits)
	HWREG BLTAPTH,	$050		; Blitter pointer to source A (high 5 bits)
	HWREG BLTAPTL,	$052		; Blitter pointer to source A (low 15 bits)
	HWREG BLTDPTH,	$054		; Blitter pointer to destination D (high 5 bits)
	HWREG BLTDPTL,	$056		; Blitter pointer to destination D (low 15 bits)
	HWREG BLTSIZE,	$058		; Blitter start and size (win/width, height)
	HWREG BLTCON0L,	$05a		; Blitter control 0 lower 8 bits (minterms)
	HWREG BLTSIZV,	$05c		; Blitter V size (for 15 bit vert size)
	HWREG BLTSIZH,	$05e		; ECS: Blitter H size & start (for 11 bit H size)
	HWREG BLTCMOD,	$060		; Blitter modulo for source C
	HWREG BLTBMOD,	$062		; Blitter modulo for source B
	HWREG BLTAMOD,	$064		; Blitter modulo for source A
	HWREG BLTDMOD,	$066		; Blitter modulo for destination D
	HWREG BLTCDAT,	$070		; Blitter source C data reg
	HWREG BLTBDAT,	$072		; Blitter source B data reg
	HWREG BLTADAT,	$074		; Blitter source A data reg
	HWREG SPRHDAT,	$078		; AGA: Ext logic UHRES sprite pointer and data identifier
	HWREG BPLHDAT,	$07a		; AGA: Ext logic UHRES bit plane identifier
	HWREG LISAID,	$07c		; AGA: Chip revision level for Denise/Lisa
	HWREG DSKSYNC,	$07e		; Disk sync pattern
	HWREG COP1LCH,	$080		; Write Copper pointer 1 (high 5 bits)
	HWREG COP1LCL,	$082		; Write Copper pointer 1 (low 15 bits)
	HWREG COP2LCH,	$084		; Write Copper pointer 2 (high 5 bits)
	HWREG COP2LCL,	$086		; Write Copper pointer 2 (low 15 bits)
	HWREG COPJMP1,	$088		; Trigger Copper 1 (any value)
	HWREG COPJMP2,	$08a		; Trigger Copper 2 (any value)
	HWREG COPINS,	$08c		; Coprocessor inst fetch identify
	HWREG DIWSTRT,	$08e		; Display window start (upper left vert-hor pos)
	HWREG DIWSTOP,	$090		; Display window stop (lower right vert-hor pos)
	HWREG DDFSTRT,	$092		; Display bitplane data fetch start.hor pos
	HWREG DDFSTOP,	$094		; Display bitplane data fetch stop.hor pos
	HWREG DMACON,	$096		; DMA control write (clear or set)
	HWREG CLXCON,	$098		; Write Sprite collision control bits
	HWREG INTENA,	$09a		; Interrupt enable bits (clear or set bits)
	HWREG INTREQ,	$09c		; Interrupt request bits (clear or set bits)
	HWREG ADKCON,	$09e		; Audio, disk and UART control
	HWREG AUD0LCH,	$0a0		; Audio channel 0 pointer (high 5 bits)
	HWREG AUD0LCL,	$0a2		; Audio channel 0 pointer (low 15 bits)
	HWREG AUD0LEN,	$0a4		; Audio channel 0 length
	HWREG AUD0PER,	$0a6		; Audio channel 0 period
	HWREG AUD0VOL,	$0a8		; Audio channel 0 volume
	HWREG AUD0DAT,	$0aa		; Audio channel 0 data
	HWREG AUD1LCH,	$0b0		; Audio channel 1 pointer (high 5 bits)
	HWREG AUD1LCL,	$0b2		; Audio channel 1 pointer (low 15 bits)
	HWREG AUD1LEN,	$0b4		; Audio channel 1 length
	HWREG AUD1PER,	$0b6		; Audio channel 1 period
	HWREG AUD1VOL,	$0b8		; Audio channel 1 volume
	HWREG AUD1DAT,	$0ba		; Audio channel 1 data
	HWREG AUD2LCH,	$0c0		; Audio channel 2 pointer (high 5 bits)
	HWREG AUD2LCL,	$0c2		; Audio channel 2 pointer (low 15 bits)
	HWREG AUD2LEN,	$0c4		; Audio channel 2 length
	HWREG AUD2PER,	$0c6		; Audio channel 2 period
	HWREG AUD2VOL,	$0c8		; Audio channel 2 volume
	HWREG AUD2DAT,	$0ca		; Audio channel 2 data
	HWREG AUD3LCH,	$0d0		; Audio channel 3 pointer (high 5 bits)
	HWREG AUD3LCL,	$0d2		; Audio channel 3 pointer (low 15 bits)
	HWREG AUD3LEN,	$0d4		; Audio channel 3 length
	HWREG AUD3PER,	$0d6		; Audio channel 3 period
	HWREG AUD3VOL,	$0d8		; Audio channel 3 volume
	HWREG AUD3DAT,	$0da		; Audio channel 3 data
	HWREG BPL1PTH,	$0e0		; Bitplane pointer 1 (high 5 bits)
	HWREG BPL1PTL,	$0e2		; Bitplane pointer 1 (low 15 bits)
	HWREG BPL2PTH,	$0e4		; Bitplane pointer 2 (high 5 bits)
	HWREG BPL2PTL,	$0e6		; Bitplane pointer 2 (low 15 bits)
	HWREG BPL3PTH,	$0e8		; Bitplane pointer 3 (high 5 bits)
	HWREG BPL3PTL,	$0ea		; Bitplane pointer 3 (low 15 bits)
	HWREG BPL4PTH,	$0ec		; Bitplane pointer 4 (high 5 bits)
	HWREG BPL4PTL,	$0ee		; Bitplane pointer 4 (low 15 bits)
	HWREG BPL5PTH,	$0f0		; Bitplane pointer 5 (high 5 bits)
	HWREG BPL5PTL,	$0f2		; Bitplane pointer 5 (low 15 bits)
	HWREG BPL6PTH,	$0f4		; Bitplane pointer 6 (high 5 bits)
	HWREG BPL6PTL,	$0f6		; Bitplane pointer 6 (low 15 bits)
	HWREG BPL7PTH,	$0f8		; AGA: Bitplane pointer 7 (high 5 bits)
	HWREG BPL7PTL,	$0fa		; AGA: Bitplane pointer 7 (low 15 bits)
	HWREG BPL8PTH,	$0fc		; AGA: Bitplane pointer 8 (high 5 bits)
	HWREG BPL8PTL,	$0fe		; AGA: Bitplane pointer 8 (low 15 bits)
	HWREG BPLCON0,	$100		; Bitplane depth and screen mode)
	HWREG BPLCON1,	$102		; Bitplane/playfield horizontal scroll values
	HWREG BPLCON2,	$104		; Sprites vs. Playfields priority
	HWREG BPLCON3,	$106		; AGA: Bitplane control reg (enhanced features)
	HWREG BPL1MOD,	$108		; Bitplane modulo (odd planes)
	HWREG BPL2MOD,	$10a		; Bitplane modulo (even planes)
	HWREG BPLCON4,	$10c		; AGA: Bitplane control reg (bitplane & sprite masks)
	HWREG CLXCON2,	$10e		; AGA: Write Extended sprite collision control bits
	HWREG BPL1DAT,	$110		; Bitplane 1 data (parallel to serial convert)
	HWREG BPL2DAT,	$112		; Bitplane 2 data (parallel to serial convert)
	HWREG BPL3DAT,	$114		; Bitplane 3 data (parallel to serial convert)
	HWREG BPL4DAT,	$116		; Bitplane 4 data (parallel to serial convert)
	HWREG BPL5DAT,	$118		; Bitplane 5 data (parallel to serial convert)
	HWREG BPL6DAT,	$11a		; Bitplane 6 data (parallel to serial convert)
	HWREG BPL7DAT,	$11c		; AGA: Bitplane 7 data (parallel to serial convert)
	HWREG BPL8DAT,	$11e		; AGA: Bitplane 8 data (parallel to serial convert)
	HWREG SPR0PTH,	$120		; Sprite 0 pointer (high 5 bits)
	HWREG SPR0PTL,	$122		; Sprite 0 pointer (low 15 bits)
	HWREG SPR1PTH,	$124		; Sprite 1 pointer (high 5 bits)
	HWREG SPR1PTL,	$126		; Sprite 1 pointer (low 15 bits)
	HWREG SPR2PTH,	$128		; Sprite 2 pointer (high 5 bits)
	HWREG SPR2PTL,	$12a		; Sprite 2 pointer (low 15 bits)
	HWREG SPR3PTH,	$12c		; Sprite 3 pointer (high 5 bits)
	HWREG SPR3PTL,	$12e		; Sprite 3 pointer (low 15 bits)
	HWREG SPR4PTH,	$130		; Sprite 4 pointer (high 5 bits)
	HWREG SPR4PTL,	$132		; Sprite 4 pointer (low 15 bits)
	HWREG SPR5PTH,	$134		; Sprite 5 pointer (high 5 bits)
	HWREG SPR5PTL,	$136		; Sprite 5 pointer (low 15 bits)
	HWREG SPR6PTH,	$138		; Sprite 6 pointer (high 5 bits)
	HWREG SPR6PTL,	$13a		; Sprite 6 pointer (low 15 bits)
	HWREG SPR7PTH,	$13c		; Sprite 7 pointer (high 5 bits)
	HWREG SPR7PTL,	$13e		; Sprite 7 pointer (low 15 bits)
	HWREG SPR0POS,	$140		; Sprite 0 vert-horiz start pos data
	HWREG SPR0CTL,	$142		; Sprite 0 position and control data
	HWREG SPR0DATA,	$144		; Sprite 0 low bitplane data
	HWREG SPR0DATB,	$146		; Sprite 0 high bitplane data
	HWREG SPR1POS,	$148		; Sprite 1 vert-horiz start pos data
	HWREG SPR1CTL,	$14a		; Sprite 1 position and control data
	HWREG SPR1DATA,	$14c		; Sprite 1 low bitplane data
	HWREG SPR1DATB,	$14e		; Sprite 1 high bitplane data
	HWREG SPR2POS,	$150		; Sprite 2 vert-horiz start pos data
	HWREG SPR2CTL,	$152		; Sprite 2 position and control data
	HWREG SPR2DATA,	$154		; Sprite 2 low bitplane data
	HWREG SPR2DATB,	$156		; Sprite 2 high bitplane data
	HWREG SPR3POS,	$158		; Sprite 3 vert-horiz start pos data
	HWREG SPR3CTL,	$15a		; Sprite 3 position and control data
	HWREG SPR3DATA,	$15c		; Sprite 3 low bitplane data
	HWREG SPR3DATB,	$15e		; Sprite 3 high bitplane data
	HWREG SPR4POS,	$160		; Sprite 4 vert-horiz start pos data
	HWREG SPR4CTL,	$162		; Sprite 4 position and control data
	HWREG SPR4DATA,	$164		; Sprite 4 low bitplane data
	HWREG SPR4DATB,	$166		; Sprite 4 high bitplane data
	HWREG SPR5POS,	$168		; Sprite 5 vert-horiz start pos data
	HWREG SPR5CTL,	$16a		; Sprite 5 position and control data
	HWREG SPR5DATA,	$16c		; Sprite 5 low bitplane data
	HWREG SPR5DATB,	$16e		; Sprite 5 high bitplane data
	HWREG SPR6POS,	$170		; Sprite 6 vert-horiz start pos data
	HWREG SPR6CTL,	$172		; Sprite 6 position and control data
	HWREG SPR6DATA,	$174		; Sprite 6 low bitplane data
	HWREG SPR6DATB,	$176		; Sprite 6 high bitplane data
	HWREG SPR7POS,	$178		; Sprite 7 vert-horiz start pos data
	HWREG SPR7CTL,	$17a		; Sprite 7 position and control data
	HWREG SPR7DATA,	$17c		; Sprite 7 low bitplane data
	HWREG SPR7DATB,	$17e		; Sprite 7 high bitplane data
	HWREG COLOR00,	$180		; Palette colour 0
	HWREG COLOR01,	$182		; Palette colour 1
	HWREG COLOR02,	$184		; Palette colour 2
	HWREG COLOR03,	$186		; Palette colour 3
	HWREG COLOR04,	$188		; Palette colour 4
	HWREG COLOR05,	$18a		; Palette colour 5
	HWREG COLOR06,	$18c		; Palette colour 6
	HWREG COLOR07,	$18e		; Palette colour 7
	HWREG COLOR08,	$190		; Palette colour 8
	HWREG COLOR09,	$192		; Palette colour 9
	HWREG COLOR10,	$194		; Palette colour 10
	HWREG COLOR11,	$196		; Palette colour 11
	HWREG COLOR12,	$198		; Palette colour 12
	HWREG COLOR13,	$19a		; Palette colour 13
	HWREG COLOR14,	$19c		; Palette colour 14
	HWREG COLOR15,	$19e		; Palette colour 15
	HWREG COLOR16,	$1a0		; Palette colour 16
	HWREG COLOR17,	$1a2		; Palette colour 17
	HWREG COLOR18,	$1a4		; Palette colour 18
	HWREG COLOR19,	$1a6		; Palette colour 19
	HWREG COLOR20,	$1a8		; Palette colour 20
	HWREG COLOR21,	$1aa		; Palette colour 21
	HWREG COLOR22,	$1ac		; Palette colour 22
	HWREG COLOR23,	$1ae		; Palette colour 23
	HWREG COLOR24,	$1b0		; Palette colour 24
	HWREG COLOR25,	$1b2		; Palette colour 25
	HWREG COLOR26,	$1b4		; Palette colour 26
	HWREG COLOR27,	$1b6		; Palette colour 27
	HWREG COLOR28,	$1b8		; Palette colour 28
	HWREG COLOR29,	$1ba		; Palette colour 29
	HWREG COLOR30,	$1bc		; Palette colour 30
	HWREG COLOR31,	$1be		; Palette colour 31
	HWREG HTOTAL,	$1c0		; AGA: Highest number count in horiz line (VARBEAMEN = 1)
	HWREG HSSTOP,	$1c2		; AGA: Horiz line pos for HSYNC stop
	HWREG HBSTRT,	$1c4		; AGA: Horiz line pos for HBLANK start
	HWREG HBSTOP,	$1c6		; AGA: Horiz line pos for HBLANK stop
	HWREG VTOTAL,	$1c8		; AGA: Highest numbered vertical line (VARBEAMEN = 1)
	HWREG VSSTOP,	$1ca		; AGA: Vert line for Vsync stop
	HWREG VBSTRT,	$1cc		; AGA: Vert line for VBLANK start
	HWREG VBSTOP,	$1ce		; AGA: Vert line for VBLANK stop
	HWREG SPRHSTRT,	$1d0		; AGA: UHRES sprite vertical start
	HWREG SPRHSTOP,	$1d2		; AGA: UHRES sprite vertical stop
	HWREG BPLHSTRT,	$1d4		; AGA: UHRES bit plane vertical start
	HWREG BPLHSTOP,	$1d6		; AGA: UHRES bit plane vertical stop
	HWREG HHPOSW,	$1d8		; AGA: DUAL mode hires H beam counter write
	HWREG HHPOSR,	$1da		; AGA: DUAL mode hires H beam counter read
	HWREG BEAMCON0,	$1dc		; Beam counter control register
	HWREG HSSTRT,	$1de		; AGA: Horizontal sync start (VARHSY)
	HWREG VSSTRT,	$1e0		; AGA: Vertical sync start (VARVSY)
	HWREG HCENTER,	$1e2		; AGA: Horizontal pos for vsync on interlace
	HWREG DIWHIGH,	$1e4		; AGA: Display window upper bits for start/stop
	HWREG BPLHMOD,	$1e6		; AGA: UHRES bit plane modulo
	HWREG SPRHPTH,	$1e8		; AGA: UHRES sprite pointer (high 5 bits)
	HWREG SPRHPTL,	$1ea		; AGA: UHRES sprite pointer (low 15 bits)
	HWREG BPLHPTH,	$1ec		; AGA: VRam (UHRES) bitplane pointer (high 5 bits)
	HWREG BPLHPTL,	$1ee		; AGA: VRam (UHRES) bitplane pointer (low 15 bits)
	HWREG FMODE, 	$1fc		; AGA: Write Fetch mode (0=OCS compatible)
	HWREG NO_OP,	$1fe		; No operation/NULL (Copper NOP instruction)

	ENDIF
	