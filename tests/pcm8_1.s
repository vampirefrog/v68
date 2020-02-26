.include doscall.mac
.include iocscall.mac

.cpu 68000

.text
	pea.l (helloStr,pc)
	DOS _PRINT
	addq.l #4, sp

	movea.l ($0088),a0
	move.l  (-$0008,a0),d0
	cmp.l   #$50434d38,d0 ;'PCM8'
	beq.s   FoundPCM8

	pea.l (notFoundStr,pc)
	DOS _PRINT
	addq.l #4, sp

	DOS _EXIT

FoundPCM8:
	pea.l (foundStr,pc)
	DOS _PRINT
	addq.l #4, sp

	pea.l (stoppingStr,pc)
	DOS _PRINT
	addq.l #4, sp

    move.l  #$0100,d0
    trap    #2

	pea.l (quittingStr,pc)
	DOS _PRINT
	addq.l #4, sp

	DOS _EXIT

.data
helloStr:
.dc.b 'PCM8 test 1', $0d, $0a, $00
notFoundStr:
.dc.b 'PCM8 Not found!', $0d, $0a, $00
foundStr:
.dc.b 'PCM8 Found!', $0d, $0a, $00
stoppingStr:
.dc.b 'Stopping PCM8 sounds', $0d, $0a, $00
quittingStr:
.dc.b 'Quitting', $0d, $0a, $00
