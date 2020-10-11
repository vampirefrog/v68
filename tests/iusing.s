.include doscall.mac
.include fefunc.mac

.cpu 68000

BUFSIZE equ 16
STRWIDTH equ 12

.text
	move.l #$7fffffff, d0
	move.l #STRWIDTH, d1
	sub.l #BUFSIZE, sp
	lea (sp), a0
	FEFUNC __IUSING

	pea.l (sp)
	DOS _PRINT
	add.l #4, sp

	add.l #BUFSIZE, sp

	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	DOS _EXIT

.data
crlf:
.dc.b $0d, $0a, $00
