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

	pea.l (helloStr,pc)
	DOS _PRINT
	addq.l #4, sp

	moveq.l	#$01,d1
	IOCS _ADPCMMOD

	moveq.l	#$00,d1
	IOCS _ADPCMMOD

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
	add.l #96, d1
	move.l	d1,-(sp)
	move.w	(pdxFd),-(sp)
	DOS	_SEEK
	addq.l	#8,sp

	* Read sample data
	move.l (sampleSize), -(sp)
	pea (sampleBlock)
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

	* Play sample!
	move.w #$0403, d1
	move.l (sampleSize), d2
	lea.l (sampleBlock), a1
	IOCS _ADPCMOUT

	* Free sample memory block
	pea	(sampleBlock)
	DOS	_MFREE
	addq.l	#4,sp

	* Normal exit
	DOS _EXIT

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