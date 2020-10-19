.include doscall.mac
.include fefunc.mac
.include iocscall.mac

.cpu 68000

BUFSIZE equ 16
STRWIDTH equ 12
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

	pea.l (trap2VectorStr,pc)
	DOS _PRINT
	addq.l #4, sp

	move.w #$22, -(sp)
	DOS _INTVCG
	addq.l #2, sp

	move.l d0, d4
	and.l #$00ffffff, d4
	sub #8, d4

	move.l d4, d0
	sub.l #BUFSIZE, sp
	lea (sp), a0
	FEFUNC __HTOS

	pea.l (sp)
	DOS _PRINT
	add.l #4, sp

	add.l #BUFSIZE, sp

	pea.l (crlf,pc)
	DOS _PRINT
	addq.l #4, sp

	move.l d4, a1
	IOCS _B_LPEEK
	cmp.l #'PCM8', d0
	beq PCM8OK

	pea.l (pcm8NotFoundStr,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT

PCM8OK:
	pea.l (pcm8FoundStr,pc)
	DOS _PRINT
	addq.l #4, sp

	sub.l #BUFSIZE, sp

	move.l #0, (sp)
	move.l #0, (sp,4)
	move.l #0, (sp,8)

	move.l #8, d1
	move.l d4, a1
	move.l sp, a2
	IOCS _B_MEMSTR
	move.l #0, (sp,8)

	pea.l (sp)
	DOS _PRINT
	addq.l #4, sp

	add.l #BUFSIZE, sp

	pea.l (crlf,pc)
	DOS _PRINT
	addq.l #4, sp

	move #$0002,-(sp)
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
	pea (sampleBlock)
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

	* Play sample!
	;move.w #$0403, d1
	;move.l (sampleSize), d2
	;lea.l (sampleBlock), a1
	;IOCS _ADPCMOUT

	move.l #2, d0
	move.l #$00080303, d1
	move.l (sampleSize), d2
	lea.l (sampleBlock), a1
	trap #2

	move.l #1, d0
	move.l #$00080403, d1
	move.l (sampleSize), d2
	lea.l (sampleBlock), a1
	trap #2

	* Free sample memory block
	pea	(sampleBlock)
	DOS	_MFREE
	addq.l	#4,sp

	DOS _EXIT

.data
trap2VectorStr:
.dc.b 'Trap 2 Vector: 0x', $00
pcm8FoundStr:
.dc.b 'PCM8 found', $0d, $0a, $00
pcm8NotFoundStr:
.dc.b 'PCM8 Not found', $0d, $0a, $00
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
crlf:
.dc.b $0d, $0a, $00

.bss
.quad

buf:
.ds.b 32
adpFd:
.ds.w 1
sampleSize:
.ds.l 1
sampleBlock:
.ds.l 1
WORK_SIZE:

.end
