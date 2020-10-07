;
; Fake HUMAN.SYS
;
; Uses a fake MMIO port to execute the actual DOS stuff
;

LineFVecAddr       equ $00002c
DOSCallVecTable    equ $001800
DOSWorkCurProcAddr equ $001caa
FakeDOSCallPort    equ $ea0000
FakeEmuCmdPort     equ $ea0008

.text

Start:
	bra DOSStart

HelloString:
	.dc.b 'Fake DOS', $0d, $0a, $00, $00

DOSStart:
	movea.l #$21000,sp
	bsr SetupDOSCalls
	; Signal to emulator that we're done initializing
	move.w #$ffff, (FakeEmuCmdPort)
@@:
	stop #$2000
	jmp @b

; Setup DOS call table at $1800
; and the Line F exception handler
SetupDOSCalls:
	lea.l	(DOSCallVecTable),a1
	move.w	#$00ff,d1
@@:
	move.l	#FakeDOSCallHandler,(a1)+ ; Fill all 256 DOS calls with the fake handler
	dbra.w	d1,@b

	move.l	#LineFExceptionHandler,(LineFVecAddr)

	rts

; Catch $Fxxx instructions
LineFExceptionHandler:
	movem.l d0-d1/a1-a2,-(sp)
	lea.l   ($0012,sp),a2  ; Get stack addr of pushed PC
	movea.l (a2),a1        ; Get PC from stack
	clr.l   d0
	move.w  (a1)+,d0       ; Get instruction from (PC)
	move.l  a1, (a2)
	cmpi.w  #$ff00,d0      ; Only process instructions >= $ff00
	bcs     LineFDone

	move.w  d0, d1
	and.l   #$000000ff, d1
	add.w   d1, d1
	add.w   d1, d1
	lea.l   (DOSCallVecTable), a2
	add.l   d1, a2
	move.l  (a2), a2
	jsr     (a2)

LineFDone:
	movem.l (sp)+,d0-d1/a1-a2
	rte

FakeDOSCallHandler:
	lea.l (26,sp),sp
	move.l #FakeDOSCallPort, a1
	move.w d0, (a1)
	lea.l (-26,sp),sp
	rts
