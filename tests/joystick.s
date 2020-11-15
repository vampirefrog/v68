.include doscall.mac

.cpu 68000

YM2151_ADDR_PORT equ $e90001
YM2151_DATA_PORT equ $e90003

.text
	pea.l (lHelloWorld,pc)
	DOS _PRINT
	addq.l #4, sp

	; Test reading from PPI
	; Port A Joystick 1
	move.l #$e9a001, a0
	move.b (a0), d0

	; Port B Joystick 2
	move.l #$e9a003, a0
	move.b (a0), d0

	; Port C ADPCM settings and other joystick bits
	move.l #$e9a005, a0
	move.b (a0), d0

	; Test writing to PPI
	; Port C write all bits
	move.l #$e9a005, a0
	move.b #$ff, d0
	move.b d0, (a0)

	; Control word, set Port C Low to output mode
	move.l #$e9a007, a0
	move.b #$80, d0
	move.b d0, (a0)

	move.l #$e9a007, a0

	move.b #$00, d0
	move.b d0, (a0)
	move.b #$01, d0
	move.b d0, (a0)
	move.b #$02, d0
	move.b d0, (a0)
	move.b #$03, d0
	move.b d0, (a0)

	; OKI div = 0
	move.b #$04, d0
	move.b d0, (a0)
	move.b #$06, d0
	move.b d0, (a0)

	move.b #$05, d0
	move.b d0, (a0)
	move.b #$06, d0
	move.b d0, (a0)

	move.b #$04, d0
	move.b d0, (a0)
	move.b #$07, d0
	move.b d0, (a0)

	move.b #$05, d0
	move.b d0, (a0)
	move.b #$07, d0
	move.b d0, (a0)

	move.w #$1b, d1
	move.w #$80, d2
	bsr WriteOPM

	move.w #$1b, d1
	move.w #$00, d2
	bsr WriteOPM

	DOS _EXIT

WriteOPM:
	tst.b	(YM2151_DATA_PORT)
	bmi.s	WriteOPM
	move.b	d1,(YM2151_ADDR_PORT)
	and.w	#$00ff,d1

	* Cache the reg write
	lea.l	(OPMCache),a2
	move.b	d2,(a2,d1.w)

	move.b	d2,(YM2151_DATA_PORT)
	cmp.b	#$1b,d1
	beq.s	_storeB1
	rts

_storeB1:
	move.b	d2,($09da)
	rts

.data
lHelloWorld:
.dc.b 'Joystick peripheral test', $0d, $0a, $00

.bss
.quad
OPMCache:
.ds.b 256
