.include doscall.mac

.cpu 68000

.text
	pea.l (lHelloWorld,pc)
	DOS _PRINT

	move.b #$00, d0
	move.b d0, ($e90001)

	DOS _EXIT

.data
lHelloWorld:
.dc.b 'Accessing MMIO without _SUPER', $0d, $0a, $00
