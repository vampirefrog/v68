.include doscall.mac
.include iocscall.mac
.include mfp.inc

.cpu 68000

YM2151_ADDR_PORT equ $e90001
YM2151_DATA_PORT equ $e90003

YM2151_CLKA_HI   equ $10
YM2151_CLKA_LO   equ $11
YM2151_CLKB      equ $12
YM2151_CLK_CTRL  equ $14


.text
	clr.l	-(sp)
	DOS _SUPER
	addq.l #4, sp

	pea.l (descstr)
	DOS _PRINT
	addq.l #4, sp

	ori.w	#$0700,sr
	* Init OPM interrupt
	lea.l	(OPMInt),a1
	suba.l	a0,a0
	move.l ($010c), (oldInt)
	move.l	a1,($010c,a0)

	* MFP init
	bclr	#3, (MFP_GPIP)
	bset	#3,	(MFP_IERB)
	bset	#3,	(MFP_IMRB)

	move.l #$00, (poop)

	* Configure timers
	move.w	#$10, d1    ; OPM reg $10, Timer A high
	move.w	#$00, d2
	bsr.w WriteOPM

	move.w	#$11, d1    ; OPM reg $10, Timer A low
	move.w	#$00, d2
	bsr.w WriteOPM

	move.w	#$12, d1    ; OPM reg $10, Timer B
	move.w	#$00, d2
	bsr.w WriteOPM

	* start the timer A and enable interrupts
	move.b #$14, d1     ; OPM reg $14, Timer bits
	move.b #$05, d2     ; Load A, IRQEN A
	bsr.w WriteOPM

	* Initialize instrument 0
	move.w #$20, d1
	move.w #$f4, d2
	bsr WriteOPM
	move.w #$28, d1
	move.w #$42, d2
	bsr WriteOPM
	move.w #$30, d1
	move.w #$14, d2
	bsr WriteOPM
	move.w #$38, d1
	move.w #$00, d2
	bsr WriteOPM
	move.w #$40, d1
	move.w #$32, d2
	bsr WriteOPM
	move.w #$48, d1
	move.w #$72, d2
	bsr WriteOPM
	move.w #$50, d1
	move.w #$71, d2
	bsr WriteOPM
	move.w #$58, d1
	move.w #$31, d2
	bsr WriteOPM
	move.w #$60, d1
	move.w #$1b, d2
	bsr WriteOPM
	move.w #$68, d1
	move.w #$1c, d2
	bsr WriteOPM
	move.w #$70, d1
	move.w #$18, d2
	bsr WriteOPM
	move.w #$78, d1
	move.w #$18, d2
	bsr WriteOPM
	move.w #$80, d1
	move.w #$18, d2
	bsr WriteOPM
	move.w #$88, d1
	move.w #$18, d2
	bsr WriteOPM
	move.w #$90, d1
	move.w #$58, d2
	bsr WriteOPM
	move.w #$98, d1
	move.w #$58, d2
	bsr WriteOPM
	move.w #$a0, d1
	move.w #$0c, d2
	bsr WriteOPM
	move.w #$a8, d1
	move.w #$0c, d2
	bsr WriteOPM
	move.w #$a8, d1
	move.w #$0c, d2
	bsr WriteOPM
	move.w #$b0, d1
	move.w #$0b, d2
	bsr WriteOPM
	move.w #$b8, d1
	move.w #$0b, d2
	bsr WriteOPM
	move.w #$c0, d1
	move.w #$03, d2
	bsr WriteOPM
	move.w #$c8, d1
	move.w #$03, d2
	bsr WriteOPM
	move.w #$d0, d1
	move.w #$00, d2
	bsr WriteOPM
	move.w #$d8, d1
	move.w #$00, d2
	bsr WriteOPM
	move.w #$e0, d1
	move.w #$10, d2
	bsr WriteOPM
	move.w #$e8, d1
	move.w #$10, d2
	bsr WriteOPM
	move.w #$f0, d1
	move.w #$15, d2
	bsr WriteOPM
	move.w #$f8, d1
	move.w #$15, d2
	bsr WriteOPM

	* Note off
	move.w #$08, d1
	move.w #$00, d2
	bsr WriteOPM

;	* Play a note
;	move.w #$08, d1
;	move.w #$78, d2
;	bsr WriteOPM

	move.l #$00, (tick_cntr)
	move.l #$00, (note_cntr)

	andi.w  #$f8ff,sr

	move.w #$0000, -(sp)
	move.l #$00010000, -(sp)
	DOS _KEEPPR

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

* OPM interrupt routine
OPMInt:
	* Disable all interrupts
	andi.b	#$f7,($00e88015)
	movem.l	d0-d7/a0-a6,-(sp)
	move.w sr, -(sp)
	andi.w	#$f8ff,sr

	* ACK ISR
	move.b #$14, d1
	move.b #$15, d2
	bsr.w WriteOPM

	* Every few ticks, write a note
	move.l (tick_cntr), d3
	add.l #1, d3
	cmp.l #30, d3
	ble skipPrint
	move.l #0, d3

;	pea.l (opmstr)
;	DOS _PRINT
;	addq.l #4, sp

	* Note off
	move.w #$08, d1
	move.w #$00, d2
	bsr WriteOPM

	move.l (note_cntr), d4
	* Key Code
	lea.l (note_tbl), a4
	move.b #$28, d1
	move.b (d4,a4), d2
	bsr.w WriteOPM

	add.l #1, d4
	andi.l #$07, d4
	move.l d4, (note_cntr)

	* Note On
	move.b #$08, d1
	move.b #$38, d2
	bsr WriteOPM

skipPrint:
	move.l d3, (tick_cntr)

	* Simply increment a value and write it to a register
	move.b #$ff, d1
	move.b (poop), d2
	add.b #1, d2
	move.b d2, (poop)
	bsr WriteOPM

	* Enable all interrupts
	move.w (sp)+, sr
	movem.l	(sp)+,d0-d7/a0-a6
	ori.b	#$08,($00e88015)
	rte

.bss
.quad
OPMCache:
.ds.b 256

oldInt:
.ds.l 1

poop:
.ds.b 4

tick_cntr:
.ds.l 1
note_cntr:
.ds.l 1

.data
note_tbl:
.dc.b $30, $32, $35, $36, $39, $3b, $3e, $40

descstr:
.dc.b 'OPM Test', $0d, $0a, $00
opmstr:
.dc.b 'OPM Int!', $0d, $0a, $00
