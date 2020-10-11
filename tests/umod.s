.include doscall.mac
.include fefunc.mac

.cpu 68000

.text
	move.l #$ffffffff, d0
	move.l #8, d1
	sub.l #100, sp
	lea (sp), a0
	FEFUNC __IUSING

	pea.l (sp)
	DOS _PRINT
	add.l #4, sp

	add.l #100, sp

	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	DOS _EXIT

.data
crlf:
.dc.b $0d, $0a, $00
