;
; Fake IOCS / IPL ROM
;
; The emulator starts execution at $000000
;

; Some constants
BusErrVecAddr             equ $000008
AddrErrVecAddr            equ $00000c
ImproperOrderVecAddr      equ $000010
PrivilegeViolationVecAddr equ $000020
TraceExceptionVecAddr     equ $000024
Trap0VecAddr              equ $000080
Trap1VecAddr              equ $000084
Trap2VecAddr              equ $000088
Trap3VecAddr              equ $00008C
Trap4VecAddr              equ $000090
Trap5VecAddr              equ $000094
Trap6VecAddr              equ $000098
Trap7VecAddr              equ $00009c
Trap8VecAddr              equ $0000a0
Trap15VecAddr             equ $0000bc
VdispVecAddr              equ $000118
CrtcVecAddr               equ $000138
DMA3IntVecAddr            equ $0001a8
DMA3ErrIntVecAddr         equ $0001ac
IOCSCallVecTable          equ $000400
IOCSWorkDMA3Mode          equ $000c32
IOCSWorkDMA3Err           equ $000c33
DefaultStackAddr          equ $002000
HumanStart                equ $006800
DMA3Regs                  equ $e840c0
I8255ControlWord          equ $e9a007
ADPCMCommand              equ $e92001
FakeIOCSCallPort          equ $ea0004

.text

IPLStart:
	.dc.l DefaultStackAddr ; Default stack top
EntryPointPtr:
	.dc.l EntryPoint       ; PC at boot
BootBusErrVec:
	.dc.l	DefaultExceptionHandler
BootAddressErrVec:
	.dc.l	DefaultExceptionHandler
BootImproperOrderVec:
	.dc.l	DefaultExceptionHandler
BootDivideBy0Vec:
	.dc.l	$00000000
BootCHKCmdVec:
	.dc.l	$00000000
BootTRAPVccVec:
	.dc.l	$00000000
BootPrivilegeViolationVec:
	.dc.l	DefaultExceptionHandler
BootTraceExceptionVec:
	.dc.l	DefaultExceptionHandler
BootLineAVec:
	.dc.l	$00000000
BootLineFVec:
	.dc.l	$00000000

EntryPoint:	             ; Boot entry point
	move.w  #$2700,sr
	lea.l   (DefaultStackAddr),a7
	reset

	bsr     SetupDevices
	bsr     SetupExceptions
	bsr     SetupTraps
	bsr     SetupInterrupts
	bsr     SetupIOCSCalls

	jmp	    HumanStart            ; Go to DOS

;
; Devices
;

SetupDevices:
	; DMAC
	move.w #$00e9, ($e840d4)
	move.w #$2003, ($e840d6)
	move.w #$0400, ($e840c6)
	; DMAC vectors
	; Channel 0
		move.b #$0f, ($e84025)
		move.b #$0f, ($e84027)
	; Channel #1
		move.b #$0f, ($e84065)
		move.b #$0f, ($e84067)
	; Channel #2
		move.b #$68, ($e840a5)
		move.b #$69, ($e840a7)
	; Channel #3
		move.b #$6a, ($e840e5)
		move.b #$6b, ($e840e7)
		; DMA default interrupts
	lea.l (DMA3Interrupt), a1
	move.l a1, (DMA3IntVecAddr)
	lea.l (DMA3Interrupt), a1
	move.l a1, (DMA3ErrIntVecAddr)

	; ADPCM
	move.b #$00, ($c32) ; playback mode

	; OPM
	move.b #$00, ($9da) ; register $1b cache

	rts

;
; Exceptions
;
SetupExceptions:
	lea.l (DefaultExceptionHandler), a1
	move.l a1, (BusErrVecAddr)
	lea.l (DefaultExceptionHandler), a1
	move.l a1, (AddrErrVecAddr)
	lea.l (DefaultExceptionHandler), a1
	move.l a1, (ImproperOrderVecAddr)
	lea.l (DefaultExceptionHandler), a1
	move.l a1, (PrivilegeViolationVecAddr)
	lea.l (DefaultExceptionHandler), a1
	move.l a1, (TraceExceptionVecAddr)
	rts

DefaultExceptionHandler:
	stop #$2000
	bra DefaultExceptionHandler
	rte

;
; IOCS Calls
;
SetupIOCSCalls:
	; Set all 256 IOCS call handlers to the fake handler
	lea.l	(IOCSCallVecTable),a1
	move.w	#$00ff,d1
@@:
	lea.l (FakeIOCSCallHandler), a2
	move.l  a2,(a1)+ ; Fill all 256 DOS calls with the fake handler
	dbra.w	d1,@b
	rts

FakeTrap15Handler:
	movem.l	a0,-(a7)
	and.l	#$000000ff,d0
	suba.l	a0,a0
	move.l	d0,a0
	adda.l	a0,a0
	adda.l	a0,a0
	adda.l	#IOCSCallVecTable,a0
	movea.l	(a0),a0
	move.w	d0,($00000a0e).l
	jsr	(a0)
	move.w	#$ffff,($00000a0e).l
	movem.l	(a7)+,a0
	rte

FakeIOCSCallHandler:
	move.w  d0, (FakeIOCSCallPort)
	rts

;
; Traps
;
SetupTraps:
	lea.l (DefaultTrapHandler), a1
	move.l a1, (Trap0VecAddr)
	lea.l (DefaultTrapHandler), a1
	move.l a1, (Trap1VecAddr)
	lea.l (DefaultTrapHandler), a1
	move.l a1, d1
	or.l #$22000000, d1
	move.l d1, (Trap2VecAddr)
	lea.l (DefaultTrapHandler), a1
	move.l a1, (Trap3VecAddr)
	lea.l (DefaultTrapHandler), a1
	move.l a1, (Trap4VecAddr)
	lea.l (DefaultTrapHandler), a1
	move.l a1, (Trap5VecAddr)
	lea.l (DefaultTrapHandler), a1
	move.l a1, (Trap6VecAddr)
	lea.l (DefaultTrapHandler), a1
	move.l a1, (Trap7VecAddr)
	lea.l (DefaultTrapHandler), a1
	move.l a1, (Trap8VecAddr)
	lea.l (FakeTrap15Handler), a1
	move.l a1, (Trap15VecAddr)
	rts

DefaultTrapHandler:
	rte

;
; Interrupts
;
SetupInterrupts:
	lea.l (DefaultIntHandler), a1
	move.l a1, (VdispVecAddr)
	lea.l (DefaultIntHandler), a1
	move.l a1, (CrtcVecAddr)
	rts

DefaultIntHandler:
	rte

DMA3Interrupt:
	move.l	a0,-(a7)
	lea.l	(DMA3Regs),a0
	move.b	($0001,a0),(IOCSWorkDMA3Err)
	tst.b	(IOCSWorkDMA3Mode)
	bmi.s	@f
	move.b	#$01,(I8255ControlWord)
	move.b	#$03,(I8255ControlWord)
	move.b	#$01,(ADPCMCommand)
@@:
	tst.b	(a0)
	st.b	(a0)
	clr.b	(IOCSWorkDMA3Mode)
	movea.l	(a7)+,a0
	rte
