.include doscall.mac

.cpu 68000

VECNUM = $02

.text
	pea.l (lHelloWorld,pc)
	DOS _PRINT
	addq.l #4, sp

	* Set our bus error handler
	pea.l (busErrHandler)
	move.w #VECNUM, -(sp)
	DOS	_INTVCS
	addq.l	#6,sp

	* Store old handler
	move.l d0, (oldHandler)

	* Access a forbidden address
	move.b #$00, d0
	move.b d0, ($e90001)

	pea.l (afterFaultStr,pc)
	DOS _PRINT
	addq.l #4, sp

	* Done
	DOS _EXIT

busErrHandler:
	* Print a message
	pea.l (busErrorStr,pc)
	DOS _PRINT
	addq.l #4, sp

	* Restore old handler
	move.l (oldHandler), -(sp)
	move #VECNUM, -(sp)
	DOS _INTVCS
	addq.l #6, sp

	DOS _EXIT

	* Return from handler
	rte

.bss
oldHandler:
.ds.l 1


.data
lHelloWorld:
.dc.b 'Accessing MMIO without _SUPER, but with a bus error vector set.', $0d, $0a, $00
busErrorStr:
.dc.b 'Bus error caught!', $0d, $0a, $00
afterFaultStr:
.dc.b 'Recovered successfully!', $0d, $0a, $00
