.include doscall.mac

.cpu 68000

.text
	pea.l (dosver)
	DOS _PRINT
	addq.l #4, sp

	DOS _VERNUM
	lsr #8, d0
	andi.l #$000000ff, d0
	bsr prtdec

	move.w #$002e, -(sp)
	DOS _PUTCHAR
	addq.l #2, sp

	DOS _VERNUM
	andi.l #$000000ff, d0
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
dosver:
.dc.b 'DOS Version: ', $00

.end
