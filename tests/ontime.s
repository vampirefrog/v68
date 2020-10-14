.include doscall.mac
.include iocscall.mac

.cpu 68000

.text
	pea.l (dosver)
	DOS _PRINT
	addq.l #4, sp

	IOCS _ONTIME
	move.l d0, d2
	move.l d1, d3

	move.l d1, d0
	bsr prtdec

	move.w #'d', -(sp)
	DOS _PUTCHAR
	addq.l #2, sp

	move.l d2, d0

	* Hours
	move.l #360000, d1
	.dc.w $fe09 * _IDIV
	bsr prtdec

	move.w #$003a, -(sp)
	DOS _PUTCHAR
	addq.l #2, sp

	* Minutes
	move.l d2, d0
	move.l #6000, d1
	.dc.w $fe09 * __IDIV
	move.l #60, d1
	.dc.w $fe06 * __UMOD
	bsr prtdec

	move.w #$003a, -(sp)
	DOS _PUTCHAR
	addq.l #2, sp

	* Seconds
	move.l d2, d0
	move.l #100, d1
	.dc.w $fe09 * __IDIV
	move.l #60, d1
	.dc.w $fe06 * __UMOD
	bsr prtdec

	move.l #':', -(sp)
	DOS _PUTCHAR
	addq.l #2, sp


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
dosver:
.dc.b 'Uptime: ', $00

.end
