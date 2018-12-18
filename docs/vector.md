```
	- INTERRUPT VECTOR MANUAL -

==============================================================================

・Exception vectors (メーカー予約:$00〜3f)

address	vector
$000000	  $00	SSP after reset
$000004	  $01	〃	     PC
$000008	  $02	Bus error / access fault
$00000c	  $03	Address error
$000010	  $04	Improper order
$000014	  $05	Division by 0
$000018	  $06	CHK command
$00001c	  $07	TRAPV、TRAPcc、cpTRAPcc command
$000020	  $08	Privilege violation
$000024	  $09	Trace exception handling
$000028	  $0a	未実装命令 line 1010 emulator (SX コール)
$00002c	  $0b	〃	   line 1111 emulator (DOS コール,浮動小数点演算)
$000030	  $0c	Unused
$000034	  $0d	FPU プロトコルバイオレーション例外処理
$000038	  $0e	〃  フォーマットエラー例外処理
$00003c	  $0f	未初期化割り込み
$000040	  $10	未使用
$000044	  $11	〃
$000048	  $12	〃
$00004c	  $13	〃
$000050	  $14	〃
$000054	  $15	〃
$000058	  $16	〃
$00005c	  $17	〃
$000060	  $18	Spurious interrupt
$000064	  $19	レベル  1 割り込み(オートベクタ時)
$000068	  $1a	〃	2 〃
$00006c	  $1b	〃	3 〃
$000070	  $1c	〃	4 〃
$000074	  $1d	〃	5 〃
$000078	  $1e	〃	6 〃
$00007c	  $1f	〃	7 〃(NMI)
$000080	  $20	trap #0
$000084	  $21	〃   #1
$000088	  $22	〃   #2
$00008c	  $23	〃   #3
$000090	  $24	〃   #4
$000094	  $25	〃   #5
$000098	  $26	〃   #6
$00009c	  $27	〃   #7
$0000a0	  $28	〃   #8  (システム予約)
$0000a4	  $29	〃   #9  (OS デバッガ用)
$0000a8	  $2a	〃   #10 (リセット／電原オフ処理)
$0000ac	  $2b	〃   #11 (BREAK キー処理)
$0000b0	  $2c	〃   #12 (COPY キー処理)
$0000b4	  $2d	〃   #13 (CTRL+C 処理)
$0000b8	  $2e	〃   #14 (エラー処理)
$0000bc	  $2f	〃   #15 (IOCS コール)
$0000c0	  $30	FPU BSUN	(Branch/Set on Unordered) 例外処理
$0000c4	  $31	〃  INEX1,INEX2	(Inexact Decimal Input,Inexact Operation)
$0000c8	  $32	〃  DZ		(Divide by Zero)
$0000cc	  $33	〃  UNFL	(Underflow)
$0000d0	  $34	〃  OPERR	(Operand Error)
$0000d4	  $35	〃  OVFL	(Overflow)
$0000d8	  $36	〃  SNAN	(Signaling Not A Number)
$0000dc	  $37	〃  UNSUPP	(Unimplemented Data Type)
$0000e0	  $38	MMU コンフィギュレーションエラー
$0000e4	  $39	〃  (68851)
$0000e8	  $3a	〃  (68851)
$0000ec	  $3b	未使用
$0000f0	  $3c	未実装実効アドレス例外処理
$0000f4	  $3d	未実装整数命令例外処理
$0000f8	  $3e	未使用
$0000fc	  $3f	〃

Remarks The vector $0c (address $30) is referenced at X68030 when determining whether it is a restart by reset.

・Exception vector (user interrupt: $40 to $ff)

address	vector
$000100	  $40	MFP RTC alarm / 1 Hz
$000104	  $41	MFP external power OFF
$000108	  $42	MFP front switch OFF
$00010c	  $43	MFP FM sound source
$000110	  $44	MFP Timer-D (used for BG processing)
$000114	  $45	MFP Timer-C (mouse / cursor / FDD control etc.)
$000118	  $46	MFP V-DISP
$00011c	  $47	MFP unused
$000120	  $48	MFP Timer-B
$000124	  $49	MFP key serial output error
$000128	  $4a	MFP key serial output sky
$00012c	  $4b	MFP key serial input error
$000130	  $4c	MFP key With serial input
$000134	  $4d	MFP Timer-A
$000138	  $4e	MFP CRTC · IRQ
$00013c	  $4f	MFP H-SYNC
$000140	  $50	SCC(B)	送信バッファ空
$000144	  $51	SCC(B)	〃 ※
$000148	  $52	SCC(B)	外部ステータス変化
$00014c	  $53	SCC(B)	〃 ※
$000150	  $54	SCC(B)	受信キャラクタ有効(マウス 1 バイト入力)
$000154	  $55	SCC(B)	〃 ※
$000158	  $56	SCC(B)	スペシャル Rx コンディション
$00015c	  $57	SCC(B)	〃 ※
$000160	  $58	SCC(A)	送信バッファ空
$000164	  $59	SCC(A)	〃 ※
$000168	  $5a	SCC(A)	外部ステータス変化
$00016c	  $5b	SCC(A)	〃 ※
$000170	  $5c	SCC(A)	受信キャラクタ有効(RS-232C 1 バイト入力)
$000174	  $5d	SCC(A)	〃 ※
$000178	  $5e	SCC(A)	スペシャル Rx コンディション
$00017c	  $5f	SCC(A)	〃 ※
$000180	  $60	I/O	FDC ステータス割り込み
$000184	  $61	I/O	FDD 挿入/排出割り込み
$000188	  $62	I/O	HDC ステータス割り込み
$00018c	  $63	I/O	プリンタレディー割り込み
$000190	  $64	DMAC	#0 終了  (FDD)
$000194	  $65	DMAC	#0 エラー(〃 )
$000198	  $66	DMAC	#1 終了  (SASI/SCSI)
$00019c	  $67	DMAC	#1 エラー(〃	   )
$0001a0	  $68	DMAC	#2 終了  (IOCS _DMAMOVE,_DMAMOV_A,_DMAMOV_L)
$0001a4	  $69	DMAC	#2 エラー(〃				   )
$0001a8	  $6a	DMAC	#3 End (ADPCM)
$0001ac	  $6b	DMAC	#3 Error (ADPCM)
$0001b0	  $6c	SPC	SCSI 割り込み(内蔵 SCSI)
$0001b4	  $6d	未使用
…	  …	…
$0001fc	  $7f	未使用
$000200	  $80	MIDI	MIDI ボード (#0) MIDI real-time message detected
$000204	  $81	MIDI	〃 ※
$000208	  $82	MIDI	〃		 Click Counter / MIDI-clock detected
$00020c	  $83	MIDI	〃 ※
$000210	  $84	MIDI	〃		 Play-back Counter
$000214	  $85	MIDI	〃 ※
$000218	  $86	MIDI	〃		 Recording Counter
$00021c	  $87	MIDI	〃 ※
$000220	  $88	MIDI	〃		 Off-line detected / Break detected
$000224	  $89	MIDI	〃 ※
$000228	  $8a	MIDI	〃		 FIFO-Rx ready
$00022c	  $8b	MIDI	〃 ※
$000230	  $8c	MIDI	〃		 FIFO-Tx empty
$000234	  $8d	MIDI	〃 ※
$000238	  $8e	MIDI	〃		 General Timer
$00023c	  $8f	MIDI	〃 ※
$000240	  $90	未使用
…	  …	…
$00027c	  $9f	未使用
$000280	  $a0	MIDI	MIDI ボード (#1) MIDI real-time message detected
$000284	  $a1	MIDI	〃 ※
$000288	  $a2	MIDI	〃		 Click Counter / MIDI-clock detected
$00028c	  $a3	MIDI	〃 ※
$000290	  $a4	MIDI	〃		 Play-back Counter
$000294	  $a5	MIDI	〃 ※
$000298	  $a6	MIDI	〃		 Recording Counter
$00029c	  $a7	MIDI	〃 ※
$0002a0	  $a8	MIDI	〃		 Off-line detected / Break detected
$0002a4	  $a9	MIDI	〃 ※
$0002a8	  $aa	MIDI	〃		 FIFO-Rx ready
$0002ac	  $ab	MIDI	〃 ※
$0002b0	  $ac	MIDI	〃		 FIFO-Tx empty
$0002b4	  $ad	MIDI	〃 ※
$0002b8	  $ae	MIDI	〃		 General Timer
$0002bc	  $af	MIDI	〃 ※
$0002c0	  $b0	RS232C	拡張 RS-232C (#0) チャンネル B
$0002c4	  $b1	〃	〃
$0002c8	  $b2	〃	〃
$0002cc	  $b3	〃	〃
$0002d0	  $b4	〃	〃
$0002d4	  $b5	〃	〃
$0002d8	  $b6	〃	〃
$0002dc	  $b7	〃	〃
$0002e0	  $b8	RS232C	拡張 RS-232C (#0) チャンネル A
$0002e4	  $b9	〃	〃
$0002e8	  $ba	〃	〃
$0002ec	  $bb	〃	〃
$0002f0	  $bc	〃	〃
$0002f4	  $bd	〃	〃
$0002f8	  $be	〃	〃
$0002fc	  $bf	〃	〃
$000300	  $c0	RS232C	拡張 RS-232C (#1) チャンネル B
$000304	  $c1	〃	〃
$000308	  $c2	〃	〃
$00030c	  $c3	〃	〃
$000310	  $c4	〃	〃
$000314	  $c5	〃	〃
$000318	  $c6	〃	〃
$00031c	  $c7	〃	〃
$000320	  $c8	RS232C	拡張 RS-232C (#1) チャンネル A
$000324	  $c9	〃	〃
$000328	  $ca	〃	〃
$00032c	  $cb	〃	〃
$000330	  $cc	〃	〃
$000334	  $cd	〃	〃
$000338	  $ce	〃	〃
$00033c	  $cf	〃	〃
$000340	  $d0	未使用
…	  …	…
$0003bc	  $ef	未使用
$0003c0	  $f0	PSX	PSX16550 / PSX16750 受信割り込み
$0003c4	  $f1	未使用
…	  …	…
$0003d4	  $f5	未使用
$0003d8	  $f6	SPC	SCSI 割り込み(SCSI ボード)
$0003dc	  $f7	TS-6BSI	パラレルポート
$0003e0	  $f8	ETHER	Nereid (#1) Ethernet
$0003e4	  $f9	ETHER	Nereid (#0) Ethernet / Neptune-X / Neptune-Evolution
	  $f9	PARA	パラレルボード (#0)
$0003e8	  $fa	PARA	パラレルボード (#1)
	  $fa	USB	Nereid (#1) USB
$0003ec	  $fb	USB	Nereid (#0) USB
$0003f0	  $fc	U-I/O	ユニバーサル I/O (#0)
$0003f4	  $fd	〃	〃		 (#1)
$0003f8	  $fe	〃	〃		 (#2)
$0003fc	  $ff	〃	〃		 (#3)
		MU	MK-MU1O(Mercury-Unit V4 OPNA) OPNA タイマ割り込み

※SCC	　SCC は各割り込みに二つずつ同じアドレスが設定されるが、実際に割り込み
	を発生させて使用するのは偶数ベクタである.
※MIDI	　MIDI ボードの割り込みは偶数ベクタのみが使用される.
※ETHER	　Neptune-X の実装によっては、DIP SW やジャンパで任意のベクタを設定で
	きる(アマルテア版、Evolution など).

==============================================================================

・割り込みベクタ設定ポート

　以下は全てバイトサイズのポート.


・DMAC

device	address	r/w
DMAC(#0)
 NIV	$e84025	R/W	%PPPP_PPPP (標準値 = $0f)
 EIV	$e84027	R/W	%PPPP_PPPP (標準値 = $0f)
DMAC(#1)
 NIV	$e84065	R/W	%PPPP_PPPP (標準値 = $0f)
 EIV	$e84067	R/W	%PPPP_PPPP (標準値 = $0f)
DMAC(#2)
 NIV	$e840a5	R/W	%PPPP_PPPP (標準値 = $68)
 EIV	$e840a7	R/W	%PPPP_PPPP (標準値 = $69)
DMAC(#3)
 NIV	$e840e5	R/W	%PPPP_PPPP (標準値 = $6a)
 EIV	$e840e7	R/W	%PPPP_PPPP (標準値 = $6b)

	NIV = Normal interrupt vector
	EIV = Error interrupt vector

・MFP

device	address	r/w
MFP	$e88017	R/W	%PPPP_XXXX (標準値 = $40)
 VR			※下位 4 ビットは割り込み要因で変化
			%0000: GPIP0		%1000: タイマ B
			%0001: GPIP1		%1001: 送信エラー
			%0010: GPIP2		%1010: 送信バッファ空
			%0011: GPIP3		%1011: 受信エラー
			%0100: タイマ D		%1100: 受信バッファフル
			%0101: タイマ C		%1101: タイマ A
			%0110: GPIP2		%1110: GPIP6
			%0111: GPIP3		%1111: GPIP7

　VR(ベクタレジスタ)のビット 3 はインサービスレジスタイネーブルの設定で、オー
ト EOI モード(%0)かソフトウェア EOI モード(%1)の選択に使用するので注意すること.
Human68k ではオート EOI モードで使用している.

・SCC

device	address	r/w
SCC(B)	$e98001	R/W	%PPPP_XXXP (標準値 = $50)
 WR2/RR2
SCC(A)	$e98005	R/W	%PPPP_XXXP (標準値 = $50)
 WR2/RR2		※ビット 3〜1 は割り込み要因で変化
			%000: チャンネル B 送信バッファ空
			%001:	〃	   外部ステータス変化
			%010:	〃	   受信キャラクタ有効
			%011:	〃	   スペシャル Rx コンディション
			%100: チャンネル A 送信バッファ空
			%101:	〃	   外部ステータス変化
			%110:	〃	   受信キャラクタ有効
			%111:	〃	   スペシャル Rx コンディション

　SCC へのベクタ番号の設定は、まず $e98001(チャンネル B)、$e98005(同 A)のコマ
ンドポートに WR2(書き込みレジスタ 2)の番号 2 を書き込み、続いて設定したいベク
タ番号を同じポートに書き込む.

　コマンドポートに 2 を書き込んでから同じポートを読むと RR2(読み込みレジスタ 2)
が読み出されるが、その内容は、チャンネル A のポートでは WR2 の設定値そのもの、
チャンネル B のポートでは最後に発生した割り込みのベクタ番号となっている.

　SCC が発生させる割り込みのベクタ番号はチャンネルと割り込み要因で 8 通りある
が、指定したベクタ番号のうちビット 3〜1 を変化させるモードと、ビット 4〜6(ビッ
トの順番が違うことに注意)を変化させるモードが選択できる. 後者の場合は上記の表
中の %PPPP_XXXP は %PXXX_PPPP となり、割り込み要因別の数値も読み換える必要があ
る. Human68k ではビット 3〜1 を変化させるモードで使用している.

・I/O コントローラ

device	address	r/w
I/O	$e9c003	R	%PPPP_PPXX  (標準値 = $60)
			※下位 2 ビットは割り込み要因で変化
			%00: FDC		%10: HD
			%01: FDD		%11: プリンタ

・MIDI ボード

device	address	r/w	(MIDI ボード)
MIDI	$eafa09	R	%PPPX_XXXX  (標準値 = $80)
 ID=0
MIDI	$eafa19	R	%PPPX_XXX0  (標準値 = $a0)
 ID=1			※ビット 4〜1 は割り込み要因で変化(ビット 0 は常に %0)
		%0000: MIDI real time message detected
		       (FIFO-IRx の最古データが $f9〜$fd)
		%0001: Click Counter(カウント値が 0 になった) /
		       MIDI-clock detected(FIFO-IRx の最古のデータが $f8)
		%0010: Play-back Counter(プレイバックカウンタが負になった)
		%0011: Recording Counter(レコーディングカウンタが 0 になった)
		%0100: Off-line detected(300mS の間受信が行われなかった) /
		       Break detected(2 キャラクタの間シリアル入力が 'L')
		%0101: FIFO-Rx ready(受信 FIFO にデータがセットされた)
		%0110: FIFO-Tx empty(送信 FIFO が空になった)
		%0111: General Timer(タイマのカウント値が 0 になった)

　$eafa03(ID=1)、$eafa13(ID=2)の RGR に $00 を書き込んでから $eafa09、$eafa19
の GRP4 にベクタ番号を書き込む.

・パラレルボード

device	address	r/w	(パラレルボード CZ-6BN1)
PARA	$eafb0b	?/W	%PPPP_PPPP (標準値 = $f9)
 ID=0
PARA	$eafb1b	?/W	%PPPP_PPPP (標準値 = $fa)
 ID=1

・拡張 RS-232C ボード

device	address	r/w	(拡張 RS-232C ボード CZ-6BF1)
RS232C	$eafc00	R/W	%PPPP_XXXP (標準値 = $b0 ※未確認)
 ID=0(B)
RS232C	$eafc04	R/W	%PPPP_XXXP (標準値 = $b8 ※未確認)
 ID=0(A)
RS232C	$eafc10	R/W	%PPPP_XXXP (標準値 = $c0 ※未確認)
 ID=1(B)
RS232C	$eafc14	R/W	%PPPP_XXXP (標準値 = $c8 ※未確認)
 ID=1(A)
RS232C	$eafc20	R/W	%PPPP_XXXP
 ID=2(B)
RS232C	$eafc24	R/W	%PPPP_XXXP
 ID=2(A)
RS232C	$eafc30	R/W	%PPPP_XXXP
 ID=3(B)
RS232C	$eafc34	R/W	%PPPP_XXXP
 ID=3(A)

　X680x0 内蔵の SCC と同一につき、SCC の項を参照のこと.

・ユニバーサル I/O ボード

device	address	r/w	(ユニバーサル I/O ボード CZ-6BU1)
UIO	$eafd03	  W	%PPPP_PPPP  (標準値 = $fc ※未確認)
 ID=0
UIO	$eafd07	  W	%PPPP_PPPP  (標準値 = $fd ※未確認)
 ID=1
UIO	$eafd0b	  W	%PPPP_PPPP  (標準値 = $fe ※未確認)
 ID=2
UIO	$eafd0f	  W	%PPPP_PPPP  (標準値 = $ff ※未確認)
 ID=3

　ID=4〜63 にも設定でき、その場合のベクタ番号設定アドレスは $eafd13〜$eafdff
となる.

・GP-IB ボード

device	address	r/w	(GP-IB ボード SH-6BG1)
GPIB	$eafe11	?/W	%PPPP_PPPP  (標準値 = $fb)

・Mercury-Unit V4

device	address	r/w	(Mercury-Unit V4 OPNA)
MK-MU1O	$ecc0b1	R/W	%PPPP_PPPP  (標準値 = $ff)
			※Master 側のタイマのみ使用可能(Slave 側は不可)

・POLYPHON

device	address	r/w	(POLYPHON)
POLY	$eff83f	R/W	%PPPP_PPPP
 ID=0
POLY	$eff8bf	R/W	%PPPP_PPPP
 ID=1

・PSX

device	address	r/w	(PSX16550 / PSX16750)
PSX	$efffxf	R/W	%PPPP_PPPP  (標準値 = $f0)
 SCR			※'x' にはチャンネル番号が入る($0〜$f)

==============================================================================
```