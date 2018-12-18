.include doscall.mac

.cpu 68000

.text
	clr.l	-(sp)
	DOS _SUPER
	addq.l #4, sp

	pea.l (lHelloWorld,pc)
	DOS _PRINT
	addq.l #4, sp

	ori.w  #$0700, sr
	andi.w #$f8ff, sr

	move.b #$00, d0
	move.b d0, ($e90001)

	DOS _EXIT

.data
lHelloWorld:
.dc.b 'Accessing MMIO with _SUPER', $0d, $0a, $00
