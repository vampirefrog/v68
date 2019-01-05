.include doscall.mac
.include iocscall.mac

.cpu 68000

STACK_SIZE: .equ 32*1024

.text
	lea		(16,a0),a0
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

	move #$0002,-(sp)
	pea	(adpcmFileStr)
	DOS	_OPEN
	addq.l	#6,sp

	move.w d0, (pdxFd)

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

	* Skip to a sample's pointer
	move.w	#$00,-(sp)
	move.l	#(3*8),-(sp)
	move.w	(pdxFd),-(sp)
	DOS	_SEEK
	addq.l	#8,sp

	move.l #4, -(sp)
	pea	(sampleOffset)
	move.w (pdxFd), -(sp)
	DOS	_READ
	lea	(10,sp),sp
	* Check for error
	tst.l d0
	bpl @f
	pea.l (readErrStr,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:

	move.l #4, -(sp)
	pea	(sampleSize)
	move.w (pdxFd), -(sp)
	DOS	_READ
	lea	(10,sp),sp
	* Check for error
	tst.l d0
	bpl @f
	pea.l (readErrStr,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:

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

	* Seek to sample data
	move.w	#$00,-(sp)
	move.l (sampleOffset), d1
;	add.l #96, d1
	move.l	d1,-(sp)
	move.w	(pdxFd),-(sp)
	DOS	_SEEK
	addq.l	#8,sp

	* Read sample data
	move.l (sampleSize), -(sp)
	move.l (sampleBlock), -(sp)
	move.w (pdxFd), -(sp)
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
	move.w (pdxFd), -(sp)
	DOS	_CLOSE
	addq.l	#4,sp

	* Write to OPM reg $1B
	move.w #$1b, d1
	move ($09da), d2
	andi.b #$7f, d2 * 8MHz
	bsr WriteOPM

	move.w #$08, -(sp)
	bsr playSample
	addq.l #2, sp

	move.w #$04, -(sp)
	bsr playSample
	addq.l #2, sp

	move.w #$00, -(sp)
	bsr playSample
	addq.l #2, sp

	* Write to OPM reg $1B
	move.w #$1b, d1
	move ($09da), d2
	ori.b #$80, d2 * 4MHz
	bsr WriteOPM

	move.w #$08, -(sp)
	bsr playSample
	addq.l #2, sp

	move.w #$04, -(sp)
	bsr playSample
	addq.l #2, sp

	move.w #$00, -(sp)
	bsr playSample
	addq.l #2, sp


	* Free sample mem block
	pea	(sampleBlock)
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


playSample:
	* Init DMA channel 3 (ADPCM)
	move.b  #$32,($00E840C5.l)
	move.b  #$FF,($00E840C0.l)
	move.l  (sampleBlock), a6
	move.l a6, ($00E840CC.l) * Address
	move.w  (sampleSize+2),($00E840CA.l) * Length
	move.b  #$88,($00E840C7.l)
	* OKI ADPCM: 1/512 divisor, pan LR
	* Write to UPD8255
	move.b (5,sp), d2
	move.b d2, ($e9a005.l)

	* OKI ADPCM: play start
	move.b  #$02, ($E92001.l)

	* Check DMAC CER errors
	move.b ($00E840C1), d2
	andi.b #$1f, d2

	* Wait for DMA transfer
@@:
	tst.w ($e840ca)
	bne @b

	; OKI ADPCM: play stop
	move.b  #$01, ($E92001.l)

	cmp.b #$01, d2
	bne @f
	pea.l (cer00001,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:
	cmp #$02, d2
	bne @f
	pea.l (cer00010,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:
	cmp #$03, d2
	bne @f
	pea.l (cer00011,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:
	cmp #$10, d2
	bne @f
	pea.l (cer10000,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:
	cmp #$11, d2
	bne @f
	pea.l (cer10001,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:
	andi.b #$1c, d2
	cmp #$04, d2
	bne @f
	pea.l (cer001rr,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:
	cmp #$08, d2
	bne @f
	pea.l (cer010rr,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:
	cmp #$0c, d2
	bne @f
	pea.l (cer011rr,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:
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
.dc.b 'OH_X.PDX', $00
couldNotOpenStr:
.dc.b 'Could not open PDX file', $0d, $0a, $00
openedFileStr:
.dc.b 'Opened PDX file', $0d, $0a, $00
crlf:
.dc.b $0d, $0a, $00

freq8: .dc.b '8MHz', $00
freq4: .dc.b '4MHz', $00
div512:  .dc.b ' / 512',  $00
div768:  .dc.b ' / 768',  $00
div1024: .dc.b ' / 1024', $00

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
pdxFd:
.ds.w 1
sampleOffset:
.ds.l 1
sampleSize:
.ds.l 1
sampleBlock:
.ds.l 1
WORK_SIZE:

.end
