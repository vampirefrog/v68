.include doscall.mac

.cpu 68000

.text

	* fd=0 STDIN
	move.w #0, -(sp)
	move.w #0, -(sp)
	DOS _IOCTRL
	addq.l #4, sp

	bsr prtdec
	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	* fd=1 STDOUT
	move.w #1, -(sp)
	move.w #0, -(sp)
	DOS _IOCTRL
	addq.l #4, sp

	bsr prtdec
	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	* fd=2 STDERR
	move.w #2, -(sp)
	move.w #0, -(sp)
	DOS _IOCTRL
	addq.l #4, sp

	bsr prtdec
	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	* fd=3 ??
	move.w #3, -(sp)
	move.w #0, -(sp)
	DOS _IOCTRL
	addq.l #4, sp

	bsr prtdec
	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	* fd=4 ??
	move.w #4, -(sp)
	move.w #0, -(sp)
	DOS _IOCTRL
	addq.l #4, sp

	bsr prtdec
	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	DOS _EXIT

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
buff:
	.ds.b 5
bufend:
	.dc.b 0

.data
crlf:
.dc.b $0d, $0a, $00
