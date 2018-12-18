.include doscall.mac

.cpu 68000

.text
.even
	move sr, -(sp)
	movem.l a0-a7, -(sp)

	pea.l (srstr)
	movem.l (0,sp), d0
	andi.l #$0000ffff, d0
	movem.l d0, -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (a0str)
	movem.l (4,sp), d0
	movem.l d0, -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (a1str)
	movem.l (8,sp), d0
	movem.l d0, -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (a2str)
	movem.l (12,sp), d0
	movem.l d0, -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (a3str)
	movem.l (16,sp), d0
	movem.l d0, -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (a4str)
	movem.l (20,sp), d0
	movem.l d0, -(sp)
	bsr printparam
	addq.l #8, sp

	* Print USP
	move.l sp, d0
	pea.l (uspstr)
	movem.l d0, -(sp)
	bsr printparam
	addq.l #8, sp

	* Go in super mode
	clr.l -(sp)
	DOS _SUPER
	addq.l #4, sp

	* Print SSP
	move.l sp, d0
	pea.l (sspstr)
	movem.l d0, -(sp)
	bsr printparam
	addq.l #8, sp

	DOS _GETPDB

	move.l d0, (pdbaddr)

	pea.l (pdbaddrstr)
	move.l (pdbaddr), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (envstr)
	move.l (pdbaddr), a0
	move.l ($00, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (envsizestr)
	move.l (pdbaddr), a0
	move.l ($00, a0), a0
	move.l (a0), -(sp)
	bsr printparam
	addq.l #8, sp

	move.l (pdbaddr), a0
	move.l (a0), a0
	addq.l #4, a0

printenva0:
	move.l a0, -(sp)
	DOS _PRINT
	addq.l #4, sp
	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

inca0:
	addq.l #1, a0
	cmp.b #0, (a0)
	bne inca0
	addq.l #1, a0
	cmp.b #0, (a0)
	bne printenva0

	pea.l (retaddrstr)
	move.l (pdbaddr), a0
	move.l ($04, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (ctrlcstr)
	move.l (pdbaddr), a0
	move.l ($08, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (errorintstr)
	move.l (pdbaddr), a0
	move.l ($0c, a0), -(sp)
	bsr printparam
	addq.l #8, sp


	pea.l (bssstartstr)
	move.l (pdbaddr), a0
	move.l ($20, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (heapstartstr)
	move.l (pdbaddr), a0
	move.l ($24, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (stackstartstr)
	move.l (pdbaddr), a0
	move.l ($28, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (parentuspstr)
	move.l (pdbaddr), a0
	move.l ($2c, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (parentsspstr)
	move.l (pdbaddr), a0
	move.l ($30, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (parentsspstr)
	move.l (pdbaddr), a0
	move.l ($30, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (parentsrstr)
	move.l (pdbaddr), a0
	move.l ($34, a0), d0
	andi.l #$0000ffff, d0
	move.l d0, -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (abortsrstr)
	move.l (pdbaddr), a0
	move.l ($36, a0), d0
	andi.l #$0000ffff, d0
	move.l d0, -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (abortsspstr)
	move.l (pdbaddr), a0
	move.l ($38, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (trap10vecstr)
	move.l (pdbaddr), a0
	move.l ($3c, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (trap11vecstr)
	move.l (pdbaddr), a0
	move.l ($40, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (trap12vecstr)
	move.l (pdbaddr), a0
	move.l ($44, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (trap13vecstr)
	move.l (pdbaddr), a0
	move.l ($48, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (trap14vecstr)
	move.l (pdbaddr), a0
	move.l ($4c, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (shellflagstr)
	move.l (pdbaddr), a0
	move.l ($50, a0), -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (modulenumstr)
	move.l (pdbaddr), a0
	move.l ($54, a0), d0
	andi.l #$000000ff, d0
	move.l d0, -(sp)
	bsr printparam
	addq.l #8, sp

	pea.l (childpspstr)
	move.l (pdbaddr), a0
	move.l ($58, a0), -(sp)
	bsr printparam
	addq.l #8, sp


	pea.l (cmdlinestr)
	DOS _PRINT
	addq.l #4, sp

	pea.l (buf)
	move.l (pdbaddr), a0
	move.l ($10, a0), -(sp)
	bsr itoh
	addq.l #8, sp

	* Write a bit of data
	move.l #8,-(sp)
	pea	(buf)
	move.l #1, d0
	move.w d0,-(sp)
	DOS	_WRITE
	lea	(10,sp),sp

	move.w #$0020, -(sp)
	DOS _PUTCHAR
	addq.l #2, sp

	move.l (pdbaddr), a0
	move.l ($10, a0), a0
	add.l #1, a0
	move.l a0, -(sp)
	DOS _PRINT
	addq.l #4, sp

	* CR LF
	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp


	pea.l (drvnamestr)
	DOS _PRINT
	addq.l #4, sp

	move.l (pdbaddr), a0
	add.l #$70, a0
	move.l a0, -(sp)
	DOS _PRINT
	addq.l #4, sp

	* CR LF
	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp


	pea.l (execnamestr)
	DOS _PRINT
	addq.l #4, sp

	move.l (pdbaddr), a0
	add.l #$b4, a0
	move.l a0, -(sp)
	DOS _PRINT
	addq.l #4, sp

	* CR LF
	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp


	DOS _EXIT

printparam:
	movem.l d0/a0-a1, -(sp)

	movem.l (20,sp), a1 * str
	movem.l (16,sp), a0 * int

	movem.l a1, -(sp)
	DOS _PRINT
	addq.l #4, sp

	pea.l (buf)
	movem.l a0, -(sp)
	bsr itoh
	addq.l #8, sp

	* Write a bit of data
	move.l #8,-(sp)
	pea	(buf)
	move.l #1, d0
	move.w d0,-(sp)
	DOS	_WRITE
	lea	(10,sp),sp

	* CR LF
	pea.l (crlf)
	DOS _PRINT
	addq.l #4, sp

	movem.l (sp)+, d0/a0-a1
	rts

include itoh.s

.bss
buf:
.ds.b 32
pdbaddr:
.ds.l 1

.data
regsstr:
.dc.b 'Registers:', $0d, $0a, $00
srstr:
.dc.b 'SR              $', $00
a0str:
.dc.b 'A0 Block ptr    $', $00
a1str:
.dc.b 'A1 Program end  $', $00
a2str:
.dc.b 'A2 Cmdline addr $', $00
a3str:
.dc.b 'A3 Env address  $', $00
a4str:
.dc.b 'A4 Entry point  $', $00
uspstr:
.dc.b 'A7 USP          $', $00
sspstr:
.dc.b 'A7 SSP          $', $00
pdbaddrstr:
.dc.b 'PDB address     $', $00
envstr:
.dc.b 'Environment     $', $00
envsizestr:
.dc.b 'Env size        $', $00
retaddrstr:
.dc.b 'Return address  $', $00
ctrlcstr:
.dc.b 'CTRL-C address  $', $00
errorintstr:
.dc.b 'Error int addr  $', $00
cmdlinestr:
.dc.b 'Cmdline         $', $00
bssstartstr:
.dc.b 'BSS Start       $', $00
heapstartstr:
.dc.b 'Heap start      $', $00
stackstartstr:
.dc.b 'Stack start     $', $00
parentuspstr:
.dc.b 'Parent USP      $', $00
parentsspstr:
.dc.b 'Parent SSP      $', $00
parentsrstr:
.dc.b 'Parent SR       $', $00
abortsrstr:
.dc.b 'Abort SR        $', $00
abortsspstr:
.dc.b 'Abort SSP       $', $00
trap10vecstr:
.dc.b 'Trap #10 vector $', $00
trap11vecstr:
.dc.b 'Trap #11 vector $', $00
trap12vecstr:
.dc.b 'Trap #12 vector $', $00
trap13vecstr:
.dc.b 'Trap #13 vector $', $00
trap14vecstr:
.dc.b 'Trap #14 vector $', $00
shellflagstr:
.dc.b 'Shell flag      $', $00
modulenumstr:
.dc.b 'Module number   $', $00
childpspstr:
.dc.b 'Child PSP       $', $00

drvnamestr:
.dc.b 'Exec path       ', $00
execnamestr:
.dc.b 'Exec name       ', $00
crlf:
.dc.b $0d, $0a, $00
