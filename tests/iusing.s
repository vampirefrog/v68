.include doscall.mac
.include fefunc.mac

.cpu 68000

BUFSIZE equ 16
STRWIDTH equ 12

.text
	pea.l (hello,pc)
	DOS _PRINT
	addq.l #4, sp

	pea.l (result,pc)
	DOS _PRINT
	addq.l #4, sp

	move.l #$7fffffff, d0
	move.l #STRWIDTH, d1
	sub.l #BUFSIZE, sp
	lea (sp), a0
	FEFUNC __IUSING

	pea.l (sp)
	DOS _PRINT
	add.l #4, sp

	add.l #BUFSIZE, sp

	pea.l (resultEnd,pc)
	DOS _PRINT
	addq.l #4, sp

	DOS _EXIT

.data
hello:
.dc.b '__IUSING test', $0d, $0a, $00
result:
.dc.b 'Result: "', $00
resultEnd:
.dc.b '"', $0d, $0a, $00
