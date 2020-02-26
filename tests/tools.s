.include doscall.mac
.include const.h

.text:

.global LoadFile
LoadFile:
	movem.l d1-d2/a1-a3,-(sp)
	move.l (24,sp), a1
	move.l (28,sp), a2
	move.l (32,sp), a3

	pea.l LoadFileLoadingStr
	DOS _PRINT
	addq.l #4, sp

	move.l a1, -(sp)
	DOS _PRINT
	addq.l #4, sp

	pea.l CRLF
	DOS _PRINT
	addq.l #4, sp

	* Open the given file
	move #$0000, -(sp)
	move.l a1, -(sp)
	DOS _OPEN
	addq.l	#6,sp
	* Return early if file desc is less than zero
	tst.l d0
	bmi LoadFileRet
	* Store file descriptor
	move.l d0, d1

	* Seek to end
	move.w #2, -(sp)
	move.l #0, -(sp)
	move.w d0, -(sp)
	DOS	_SEEK
	addq.l #8, sp
	* Store file size
	move.l d0, (a3)
	* Rewind
	move.w #0, -(sp)
	move.l #0, -(sp)
	move.w d1, -(sp)
	DOS _SEEK
	addq.l #8, sp

	* Allocate mem
	move.l (a3), -(sp)
	DOS _MALLOC
	addq.l #4, sp
	* Check error
	tst.l d0
	bmi LoadFileClose
	move.l d0, (a2)

	* Read data into memory
	move.l (a3), -(sp)
	move.l (a2), -(sp)
	move d1,-(sp)
	DOS _READ
	lea (10,sp),sp
	* Check error
	tst.l d0
	bmi LoadFileFree

	bra LoadFileClose

LoadFileFree:
	move.l (a2), -(sp)
	DOS _MFREE
	addq.l #4, sp
LoadFileClose:
	move d1, -(sp)
	DOS _CLOSE
	addq.l #2, sp
LoadFileRet:
	movem.l (sp)+,d1-d2/a1-a3
	rts

.global WriteOPM

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
	beq.s	_storeB1
	rts

_storeB1:
	move.b	d2,($09da)
	rts

.data
LoadFileLoadingStr:
	.dc.b 'Loading file ', 0
CRLF:
	.dc.b CR, LF, 0
