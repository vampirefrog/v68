.include doscall.mac
.include iocscall.mac

.cpu 68000

STACK_SIZE: .equ 32*1024

.text
	lea		(16,a0),a0
	move.l		a1, sp
	adda.l		#STACK_SIZE, sp
	suba.l		a0,a1
	adda.l		#WORK_SIZE+STACK_SIZE,a1
	move.l		a1,-(sp)
	move.l		a0,-(sp)
	DOS		_SETBLOCK
	addq.l		#8,sp
	tst.l d0
	bpl @f
	pea.l (setBlockErrStr,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:

	* Become superman
	clr.l	-(sp)
	DOS _SUPER
	addq.l #4, sp

	pea.l (helloStr,pc)
	DOS _PRINT
	addq.l #4, sp

	* _ADPCMMOD d1=$01 equivalent
	* DMA 3 CCR $00E840C7
	*  bit 5 HLT = 1
	ori.w #$0700, sr
	move.b #$20, ($00E840C7.l)

	* _ADPCMMOD d1=$00 equivalent
	clr.w ($00000C32.l)
	move.b #$01, ($E92001.l)
	ori.w #$0700, sr
	move.b #$10,($00E840C7.l)

	move #$0000,-(sp)
	pea	(adpcmFileStr)
	DOS	_OPEN
	addq.l	#6,sp

	move.w d0, (adpFd)

	cmp.l #$00, d0
	bge fileOpened

	pea.l (couldNotOpenStr,pc)
	DOS _PRINT
	addq.l #4, sp

	DOS _EXIT

fileOpened:
	pea.l (openedFileStr,pc)
	DOS _PRINT
	addq.l #4, sp

	* Skip to end, get size
	move.w	#$02,-(sp)
	move.l	#$00,-(sp)
	move.w	(adpFd),-(sp)
	DOS	_SEEK
	move.l	d0, (sampleSize)
	addq.l	#8,sp

	* rewind
	move.w	#$00,-(sp)
	move.l	#$00,-(sp)
	move.w	(adpFd),-(sp)
	DOS	_SEEK
	addq.l	#8,sp

	* Allocate memory for the sample
	move.l (sampleSize), -(sp)
	DOS	_MALLOC
	addq.l	#4,sp

	move.l d0, (sampleBlock)

	tst.l d0
	bpl @f
	pea.l (mallocErrStr,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:

	* Read sample data
	move.l (sampleSize), -(sp)
	move.l (sampleBlock), -(sp)
	move.w (adpFd), -(sp)
	DOS _READ
	lea (10,sp),sp
	* Check for error
	tst.l d0
	bpl @f
	pea.l (readSampleErrStr,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:
	* Close PDX file
	move.w (adpFd), -(sp)
	DOS	_CLOSE
	addq.l	#4,sp

	* Init DMA channel 3 (ADPCM)
	move.b  #$32,($00E840C5.l) ; Channel 3, OCR
	move.b  #$FF,($00E840C0.l) ; Channel 3, CSR
	move.l  (sampleBlock), a6
	move.l a6, ($00E840CC.l)   ; Channel 3, MAR
	move.w  (sampleSize+2),($00E840CA.l) * Length
	* OKI ADPCM: 1/512 divisor, pan LR
	* Write to UPD8255
	move.b  #$08, ($e9a005.l)
	* Write to OPM reg $1B
	move.w #$1b, d1
	move ($09da), d2
	* ori.b #$80, d2 * 4MHz
	andi.b #$7f, d2 * 8MHz
	bsr WriteOPM

	; Setup interrupts
	lea.l (DMAIntHandler), a6
	move.l a6, ($0001A8)
	lea.l (DMAErrIntHandler), a6
	move.l a6, ($0001AC)

	* OKI ADPCM: play start
	move.b  #$02, ($E92001.l)

	* Start DMA transfer
	move.b  #$88,($00E840C7.l) ; Channel 3, CCR

	* Check for DMA errors
	bsr DMAErrCheck
	tst.l d0
	* If D0 is non zero, exit
	beq @f
	DOS _EXIT

	* Wait for DMA transfer
@@:
	tst.w (dmaDone)
	beq @b

	; OKI ADPCM: play stop
	move.b  #$01, ($E92001.l)

	* Free sample mem block
	move.l (sampleBlock), -(sp)
	DOS	_MFREE
	addq.l	#4,sp

	* Normal exit
	DOS _EXIT

WriteOPM:
	ori.w	#$0300,sr
	tst.b	($00e90003)
	bmi.s	WriteOPM
	move.b	d1,($00e90001)
	and.w	#$00ff,d1
	move.b	d2,($00e90003)
	andi.w	#$faff,sr
	cmp.b	#$1b,d1
	beq.s	@f
	rts

@@:
	move.b	d2,($09da)
	rts

DMAIntHandler:
	andi.b	#$f7,($00e88015)
	movem.l	d0-d7/a0-a6,-(sp)
	move.w sr, -(sp)
	andi.w	#$f8ff,sr

	pea.l (dmaIntHandlerStr,pc)
	DOS _PRINT
	addq.l #4, sp

	move.b ($e840c0), d0

	* print out CSR
	pea.l (buf2)
	movem.l d0, -(sp)
	bsr itoh
	addq.l #8, sp


	pea.l (dmaCsr)
	DOS _PRINT
	addq.l #4, sp
	move.l #8,-(sp)
	pea	(buf2)
	move.l #1, d0
	move.w d0,-(sp)
	DOS	_WRITE
	lea	(10,sp),sp
	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	* Print errors, if any
	bsr DMAErrCheck

	* Enable all interrupts
	move.w (sp)+, sr
	movem.l	(sp)+,d0-d7/a0-a6
	ori.b	#$08,($00e88015)

	move.b #1, (dmaDone)

	rte

DMAErrIntHandler:
	andi.b	#$f7,($00e88015)
	movem.l	d0-d7/a0-a6,-(sp)
	move.w sr, -(sp)
	andi.w	#$f8ff,sr

	pea.l (dmaErrIntHandlerStr,pc)
	DOS _PRINT
	addq.l #4, sp

	* Enable all interrupts
	move.w (sp)+, sr
	movem.l	(sp)+,d0-d7/a0-a6
	ori.b	#$08,($00e88015)

	move.b #1, (dmaDone)

	rte

DMAErrCheck:
	* Check DMAC CER errors
	move.b ($00E840C1), d2     ; Channel 3, CER
	andi.b #$1f, d2

	cmp.b #$01, d2
	bne @f
	pea.l (cer00001,pc)
	DOS _PRINT
	addq.l #4, sp
	move.l #1, d0
	rts

@@:
	cmp #$02, d2
	bne @f
	pea.l (cer00010,pc)
	DOS _PRINT
	addq.l #4, sp
	move.l #1, d0
	rts

@@:
	cmp #$03, d2
	bne @f
	pea.l (cer00011,pc)
	DOS _PRINT
	addq.l #4, sp
	move.l #1, d0
	rts

@@:
	cmp #$10, d2
	bne @f
	pea.l (cer10000,pc)
	DOS _PRINT
	addq.l #4, sp
	move.l #1, d0
	rts

@@:
	cmp #$11, d2
	bne @f
	pea.l (cer10001,pc)
	DOS _PRINT
	addq.l #4, sp
	move.l #1, d0
	rts

@@:
	andi.b #$1c, d2
	cmp #$04, d2
	bne @f
	pea.l (cer001rr,pc)
	DOS _PRINT
	addq.l #4, sp
	move.l #1, d0
	rts

@@:
	cmp #$08, d2
	bne @f
	pea.l (cer010rr,pc)
	DOS _PRINT
	addq.l #4, sp
	move.l #1, d0
	rts

@@:
	cmp #$0c, d2
	bne @f
	pea.l (cer011rr,pc)
	DOS _PRINT
	addq.l #4, sp
	move.l #1, d0
	rts

@@:
	move.l #0, d0
	rts

include itoh.s

.data
helloStr:
.dc.b 'ADPCM test', $0d, $0a, $00
setBlockErrStr:
.dc.b 'Could not SETBLOCK', $0d, $0a, $00
readErrStr:
.dc.b 'Could not READ', $0d, $0a, $00
readSampleErrStr:
.dc.b 'Could not READ sample data', $0d, $0a, $00
mallocErrStr:
.dc.b 'Could not MALLOC', $0d, $0a, $00
adpcmFileStr:
.dc.b 'piano-c4.adp', $00
couldNotOpenStr:
.dc.b 'Could not open PDX file', $0d, $0a, $00
openedFileStr:
.dc.b 'Opened PDX file', $0d, $0a, $00
dmaIntHandlerStr:
.dc.b 'DMA Interrupt Handler', $0d, $0a, $00
dmaErrIntHandlerStr:
.dc.b 'DMA Error Interrupt Handler', $0d, $0a, $00
dmaCsr:
.dc.b 'CSR=', $00
crlf:
.dc.b $0d, $0a, $00

* DMA CER errors:
cer00001: .dc.b 'Configuration error', $0d, $0a, $00
cer00010: .dc.b 'Operation timing error', $0d, $0a, $00
cer00011: .dc.b '(not used)', $0d, $0a, $00
cer001rr: .dc.b 'Address error', $0d, $0a, $00
cer010rr: .dc.b 'bus error', $0d, $0a, $00
cer011rr: .dc.b 'Count error', $0d, $0a, $00
cer10000: .dc.b 'External forced stop', $0d, $0a, $00
cer10001: .dc.b 'software forced stop', $0d, $0a, $00


.bss
.quad

buf:
.ds.b 32
buf2:
.ds.b 100
adpFd:
.ds.w 1
sampleOffset:
.ds.l 1
sampleSize:
.ds.l 1
sampleBlock:
.ds.l 1

oldDMACInt:
.ds.l 1
oldDMACErrInt:
.ds.l 1

dmaDone:
.dc.b 1

WORK_SIZE:

.end
