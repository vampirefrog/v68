.include doscall.mac
.include iocscall.mac

.cpu 68000

.text
Start:
.include startup.s

	; Become supervisor
	clr.l	-(sp)
	DOS _SUPER
	addq.l #4, sp

	; Greet everyone
	pea.l (helloStr,pc)
	DOS _PRINT
	addq.l #4, sp

	move #$0000,-(sp)
	pea	(adpcmC4File)
	DOS	_OPEN
	addq.l	#6,sp

	move.w d0, (adpcmC4Fd)

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
	move.w	#$02,-(sp)
	move.l	#$00,-(sp)
	move.w	(pianoC4Fd),-(sp)
	DOS	_SEEK
	addq.l	#8,sp

	move.l (pianoC4Size), d0

	move.w	#$00,-(sp)
	move.l	#$00,-(sp)
	move.w	(pianoC4Fd),-(sp)
	DOS	_SEEK
	addq.l	#8,sp

	* Allocate memory for the sample
	move.l (pianoC4Size), -(sp)
	DOS	_MALLOC
	addq.l	#4,sp

	move.l d0, (pianoC4Block)

	tst.l d0
	bpl @f
	pea.l (mallocErrStr,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:

	* Read sample data
	move.l (pianoC4Size), -(sp)
	move.l (pianoC4Block), -(sp)
	move.w (pianoC4Fd), -(sp)
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
	move.w (pianoC4Fd), -(sp)
	DOS	_CLOSE
	addq.l	#4,sp

	* Free sample mem block
	move.l (pianoC4Block), -(sp)
	DOS	_MFREE
	addq.l	#4,sp

	* Normal exit
	DOS _EXIT

WriteOPM:
	ori.w	#$0300,sr
@@:
	tst.b	($00e90003)
	bmi.s	@b
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

include itoh.s

.data
helloStr:
.dc.b 'PCM8 test 1', $0d, $0a, $00
setBlockErrStr:
.dc.b 'Could not SETBLOCK', $0d, $0a, $00
readErrStr:
.dc.b 'Could not READ', $0d, $0a, $00
readSampleErrStr:
.dc.b 'Could not READ sample data', $0d, $0a, $00
mallocErrStr:
.dc.b 'Could not MALLOC', $0d, $0a, $00
pianoC4File:
.dc.b 'piano-c4.adp', $00
pianoE4File:
.dc.b 'piano-e4.adp', $00
pianoG4File:
.dc.b 'piano-g4.adp', $00

couldNotOpenStr:
.dc.b 'Could not open ADPCM file', $0d, $0a, $00
openedFileStr:
.dc.b 'Opened ADPCM file', $0d, $0a, $00
crlf:
.dc.b $0d, $0a, $00

.bss
.quad

buf:
.ds.b 32
pianoC4fd:
.ds.w 1
pianoE4fd:
.ds.w 1
pianoG4fd:
.ds.w 1
pianoC4Block:
.ds.l 1
pianoE4Block:
.ds.l 1
pianoG4Block:
.ds.l 1
pianoC4Size:
.ds.l 1
pianoE4Size:
.ds.l 1
pianoG4Size:
.ds.l 1
WORK_SIZE:

.end
