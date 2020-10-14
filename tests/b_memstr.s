.include iocscall.mac
.include doscall.mac

.cpu 68000

.text
	move.l #8, d1
	lea (src,pc), a1
	lea (dst,pc), a2
	IOCS _B_MEMSTR

	nop
	nop
	nop

	pea.l (dst,pc)
	DOS _PRINT
	addq.l #4, sp

	DOS _EXIT

.data
src:
.dc.b 'abcd12', $0d, $0a
.dc.b 0

.bss
dst:
.ds.b 8
dstend:
.dc.b 0
