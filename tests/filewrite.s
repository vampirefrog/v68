.include doscall.mac

.cpu 68000

.text
	pea.l (openstr)
	DOS _PRINT
	addq.l #4, sp

	* Open ye file
	move #$0001, -(sp)
	pea(filename)
	DOS _OPEN
	addq.l #6, sp

	move.l d0, (fileno)

	* if open succeeded, go to good
	cmp.l #0, d0
	bge.s good

	* try creating the file if open failed
	move.w #$0000, -(sp)
	pea(filename)
	DOS _CREATE
	addq.l #6, sp

	move.l d0, (fileno)

	cmp.l #0, d0
	blt.s err1

good:

	pea.l (successstr)
	DOS _PRINT
	addq.l #4, sp

	move.l (fileno), d0
	bsr prtdec
	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	* Write a bit of data
	move.l #(dataend-data),-(sp)
	pea	(data)
	move.l (fileno), d0
	move d0,-(sp)
	DOS	_WRITE
	lea	(10,sp),sp

	pea.l (closestr)
	DOS _PRINT
	addq.l #4, sp

	* Close ye file
	move.l (fileno), d0
	move.l d0, -(sp)
	DOS _CLOSE
	addq.l #4, sp

	DOS _EXIT

err1:
	pea.l (openerr)
	DOS _PRINT
	addq.l #4, sp

	move.l (fileno), d0
	bsr prtdec
	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	move.w #$ffff, -(sp)
	DOS _EXIT2

prtdec:
	movem.l d0/a0,-(sp)
	andi.l #$0000ffff,d0
	lea.l  bufend,a0
prtdec0:
	divu.w #10,d0
	swap.w d0
	addi.w #'0', d0
	move.b d0,-(a0)
	clr.w d0
	swap.w d0
	bne prtdec0

	move.l a0,-(sp)
	DOS _PRINT
	addq.l #4,sp

	movem.l (sp)+,d0/a0
	rts



.bss
fileno:
	.dc.l 0
buff:
	.ds.b 5
bufend:
	.dc.b 0



.data
crlf:
.dc.b $0d, $0a, $00
openstr:
.dc.b 'Opening file', $0d, $0a, $00
successstr:
.dc.b 'File successfully opened: ', $00
closestr:
.dc.b 'Closing file', $0d, $0a, $00
filename:
.dc.b 'write.txt', $00
openerr:
.dc.b 'Could not open file: ', $00

data:
.dc.b 'Hello world!', $0d, $0a
dataend:
