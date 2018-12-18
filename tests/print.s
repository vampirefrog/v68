.include doscall.mac

.cpu 68000

.text
	pea.l (lHelloWorld,pc)
	DOS _PRINT
	addq.l #4, sp

	DOS _EXIT

.bss
.dc.l 0

.data
lHelloWorld:
.dc.b 'Hello world!', $0d, $0a, $00
