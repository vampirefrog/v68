.include doscall.mac

.cpu 68000

.text
.even
	pea.l (hello)
	DOS _PRINT
	addq.l #4, sp

	addq.l #1, a2

	movem.l a2, -(sp)
	DOS _PRINT
	addq.l #4, sp

	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	* Print return value
	pea.l (retstr)
	DOS _PRINT
	addq.l #4, sp

	pea	(buf)
	movem.l a2, -(sp)
	DOS	_NAMECK
	addq.l	#8,sp

	pea.l (buf2)
	movem.l d0, -(sp)
	bsr itoh
	addq.l #8, sp

	* Write a bit of data
	move.l #8,-(sp)
	pea	(buf2)
	move.l #1, d0
	move.w d0,-(sp)
	DOS	_WRITE
	lea	(10,sp),sp

	* CR LF
	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	* Drive name
	pea.l (drivestr)
	DOS _PRINT
	addq.l #4, sp

	pea (buf)
	DOS _PRINT
	addq.l #4, sp

	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	* File
	pea.l (filestr)
	DOS _PRINT
	addq.l #4, sp

	lea (buf), a0
	add.l #67, a0
	move.l a0, -(sp)
	DOS _PRINT
	addq.l #4, sp

	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp


	* Extension
	pea.l (extstr)
	DOS _PRINT
	addq.l #4, sp

	lea (buf), a0
	add.l #86, a0
	move.l a0, -(sp)
	DOS _PRINT
	addq.l #4, sp

	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	DOS _EXIT

include itoh.s


.bss
buf:
.ds.b 100
buf2:
.ds.b 100

.data
hello:
.dc.b 'NAMECK ', $00
retstr:
.dc.b 'Return: $', $00
drivestr:
.dc.b 'Drive + path: ', $00
filestr:
.dc.b 'File name: ', $00
extstr:
.dc.b 'Extension: ', $00
crlf:
.dc.b $0d, $0a, $00
