	; Setup stack and block size
	; Include this file at entry point
STACK_SIZE: .equ 32*1024
	; a0 - Address of memory management pointer
	; a1 - End address of program + 1

	; Setup stack pointer
	move.l a1, a7
	add.l  STACK_SIZE, a7

	; Reduce memory block
	; So we can use malloc
	lea     (16,a0),a0
	suba.l		a0,a1
	adda.l		#(WORK_SIZE-Start)+STACK_SIZE,a1
	move.l		a1,-(sp)
	move.l		a0,-(sp)
	DOS		_SETBLOCK
	addq.l		#8,sp
	tst.l d0
	bpl @f
	pea.l (setBlockErrStr,pc)
	DOS _PRINT
	addq.l #4, sp
	DOS _EXIT
@@:
