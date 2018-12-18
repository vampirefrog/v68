.include doscall.mac

.cpu 68000

.text
	* Date
	pea.l (datestr)
	DOS _PRINT
	addq.l #4, sp

	DOS _GETDATE
	move.l d0, d1
	lsr.l #8, d0
	lsr.l #1, d0
	andi.l #$0000007f, d0
	addi.w #1980, d0
	bsr prtdec

	move.w #$002d, -(sp)
	DOS _PUTCHAR
	addq.l #2, sp

	move.l d1, d0
	lsr.l #5, d0
	andi.l #$0000000f, d0
	bsr prtdec

	move.w #$002d, -(sp)
	DOS _PUTCHAR
	addq.l #2, sp

	move.l d1, d0
	andi.l #$00000001f, d0
	bsr prtdec

	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp


	* Time
	pea.l (timestr)
	DOS _PRINT
	addq.l #4, sp

	DOS _GETTIME
	move.l d0, d1
	lsr.l #8, d0
	lsr.l #3, d0
	andi.l #$0000001f, d0
	bsr prtdec

	move.w #$003a, -(sp)
	DOS _PUTCHAR
	addq.l #2, sp

	move.l d1, d0
	lsr.l #5, d0
	andi.l #$0000003f, d0
	bsr prtdec

	move.w #$003a, -(sp)
	DOS _PUTCHAR
	addq.l #2, sp

	move.l d1, d0
	andi.l #$00000001f, d0
	lsl.l #1, d0
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
datestr:
.dc.b 'Date: ', $00
timestr:
.dc.b 'Time: ', $00

* monstr:
* .dc.b 'Monday', $00
* tuestr:
* .dc.b 'Tuesday', $00
* wedstr:
* .dc.b 'Wednesday', $00
* thustr:
* .dc.b 'Thursday', $00
* fristr:
* .dc.b 'Friday', $00
* satstr:
* .dc.b 'Saturday', $00
* sunstr:
* .dc.b 'Sunday' $00
* weekptrs:
* .dc.l monstr, tuestr, wedstr, thustr, fristr, satstr, sunstr

.end
