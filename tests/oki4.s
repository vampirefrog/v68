.include doscall.mac
.include iocscall.mac
.include const.h
.include tools.h

STACK_SIZE = 32*1024
.text
	move.l a1, a7
	adda.l #STACK_SIZE, a7
	lea (16,a0),a0
	suba.l a0,a1
	adda.l #STACK_SIZE,a1
	move.l a1,-(sp)
	move.l a0,-(sp)
	DOS _SETBLOCK
	addq.l #8,sp

	pea dataSize
	pea dataBlock
	pea filename
	bsr LoadFile
	lea (12,sp),sp

	pea dataSize2
	pea dataBlock2
	pea filename2
	bsr LoadFile
	lea (12,sp),sp

	moveq.l	#$01,d1
	IOCS _ADPCMMOD

	moveq.l	#$00,d1
	IOCS _ADPCMMOD

	* Play sample!
	move.w #$0403, d1
	move.l (dataSize), d2
	move.l (dataBlock), a1
	IOCS _ADPCMOUT

	move.w #$0403, d1
	move.l (dataSize2), d2
	move.l (dataBlock2), a1
	IOCS _ADPCMOUT

	move.w #$0100,d0
	trap #2

	pea.l (bye,pc)
	DOS _PRINT
	addq.l #4, sp

	DOS _EXIT

.data
hello:
.dc.b 'Hello', CR, LF, 0
bye:
.dc.b 'Done', CR, LF, 0
filename:
.dc.b 'piano-c4.adp', 0
filename2:
.dc.b 'piano-e4.adp', 0

.bss
dataBlock:
.ds.l 1
dataSize:
.ds.l 1
dataBlock2:
.ds.l 1
dataSize2:
.ds.l 1
