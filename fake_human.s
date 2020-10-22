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

; Catch $Fxxx illegal instructions
LineFExceptionHandler:
	movem.l d5-d6/a5-a6,-(sp)
	lea.l   ($0012,sp),a6  ; Get stack addr of pushed PC
	movea.l (a6),a5        ; Get PC from stack
	move.w  (a5)+,d5       ; Get instruction from (PC)
	move.l  a5, (a6)+
	cmpi.w  #$fe00, d5     ; Only process instructions >= $fe00
	bcs     LineFDone
	cmpi.w  #$ff00, d5
	bcs     LineFE
	move.l  #0, d6
	move.b  d5, d6
	lea.l	(DOSCallVecTable),a5
	add.l   d6, d6
	add.l   d6, d6
	add.l	d6, a5
	movea.l	(a5),a5
	jsr	    (a5)

	; move.w	sr,d6
	; move.b	d6,($000d,sp)

LineFDone:
	movem.l	(sp)+,d5-d6/a5-a6

	tst.w	(sp)
	bmi.s	@f
	rte

@@:
	ori.w	#$8000,sr
	rte

LineFE:
	move.l #FakeDOSCallPort, a5
	and.l   #$0000ffff, d5
	move.w d5, (a5)
	movem.l	(sp)+,d5-d6/a5-a6
	rte

FakeDOSCallHandler:
	lea.l (26,sp),sp
	move.l #FakeDOSCallPort, a6
	move.w d5, (a6)
	lea.l (-26,sp),sp
	rts

SpecialFiles:
.dc.l $ffffffff
.dc.w $8024
.dc.l $0
.dc.l $0
.dc.b 'NUL     '
.dc.l $00000000
