.include doscall.mac

.cpu 68000

.text

	move.w #$10, d5
	move.w #$40, d6
loop3:
	move.w #$10, d7
loop2:
	move.w #$f3, -(sp)
	DOS _PUTCHAR
	addq.l #2, sp
	movem.w d6, -(sp)
	DOS _PUTCHAR
	addq.l #2, sp
	add.w #1, d6
	sub.w #1, d5
	bcs loop2

	move.w #$0d, -(sp)
	DOS _PUTCHAR
	addq.l #2, sp
	move.w #$0a, -(sp)
	DOS _PUTCHAR
	addq.l #2, sp

	sub.w #1, d7
	bcs loop3

	DOS _EXIT



.end
