```
===============================================================================

	Real time・ＡＤＰＣＭ multiple playback driver ー  "ＰＣＭ８"  version 0.48

				(( technical data ))
							(C) 江藤　啓 1991,92

===============================================================================

- About PCM data format -------------------------------------------- ---------


	The format of PCM data handled by PCM 8 is the following three types.


	· ADPCM format

	A format that can be handled by ordinary ADPCM driver, IOCS, etc.
	Five types of playback frequencies can be selected.

	· 16 bit PCM format

	The data format is signed short, and by PCM 2 PCM.X (by Yatsube.)
	It is the same as the format generated.

	In PCM 8, this type of data block is set from an even address, and
	The length of the data also needs to be an even number.

	It requires four times the data amount of ADPCM format data of the same tone length.
	The playback frequency is fixed at 15.6 kHz.

	· 8 bit PCM format

	The data format is signed char, the D / A conversion level is 16 bit PCM and
	Are the same.

	It requires twice the data amount of ADPCM format data of the same tone length.
	The playback frequency is fixed at 15.6 kHz.


-- PCM8 Function call ( TRAP #2 ) -----------------------------------


	It is realized using TRAP # 2.

	Set the function code in d0.w and issue TRAP # 2 after
	setting appropriate values to other registers if necessary.

	The return value is set to d0.l. All other registers are saved.

	Note) The following contents are at the time of version 0.48.
	In some earlier versions some behaviors may be different.

----------------------------------------
Normal output        Function code $000x
----------------------------------------

Func:	Output data of single memory area to ADPCM.

Call:	d0.w = $0000 + channel number(0〜7)

	d1.l = volume / frequency / localization

	    bit 23〜16 : Volume (-1,0〜15) (*1)
		If -1 is specified, the previous value is retained.
		Original volume is 8.
		It corresponds to 1 step = 2 dB (-16dB〜+14dB)。

	    bit 15〜 8 : frequency, data format (-1,0〜4,5,6)
		If -1 is specified, the previous value is retained.
		0〜4 are ADPCM, and the reproduction frequency is as follows.
		    0 : 3.9KHz	3 : 10.4KHz
		    1 : 5.2KHz	4 : 15.6KHz
		    2 : 7.8KHz
		5 is 16 bit PCM, and 6 is 8 bit PCM. (*2)
		    Refer to "About PCM data format" for these.

	    bit  7〜 0 : pan (-1,0〜3)
		If -1 is specified, the previous value is retained.
		Other values ​​are as follows.
		    0 : Stop(*3)     2 : Right output
		    1 : Left output	 3 : Stereo output
		Note that the localization is common to all channels,
		and the last specified value is valid except for 0.

	d2.l = Data length
	    Regardless of the data format, specify it in byte units.
	    <0 : Query data length (equivalent to function code $008x)
	    =0 : Channel stop (*4)

	a1.l = Data block start address

Ret:	d0.l = 0	Successful completion

Note:
(* 1) In the single sound reproduction mode, regardless of the setting, it is output at the original volume.
(* 2) It is not output in the single sound reproduction mode.
(* 3) The volume / frequency is changed. Positioning is not changed.
(* 4) Volume / frequency / localization is not changed.

----------------------------------------
Array chain output Func code $001x
----------------------------------------

Func:	チェインテーブルによって間接的に指定された複数メモリ領域のデータを
	ＡＤＰＣＭに出力する。

Call:	d0.w = $0010 + Channel number(0〜7)
	d1.l = Volume / frequency / localization
	    >> Reference normal output ($000x)
	d2.l = チェインテーブル個数
	    <0 : データ長問い合わせ(Func code$008xと同等)
	    =0 : チャネル停止(*4)
	a1.l = チェインテーブル先頭アドレス

	*チェインテーブルについての説明は省略。

Ret:	d0.l = 0	Success

----------------------------------------
Link chain output       Func code $002x
----------------------------------------

Func:	And outputs data of a plurality of memory areas indirectly designated by the link chain table to the ADPCM.

Call:	d0.w = $0020 + Channel number(0〜7)
	d1.l = Volume / frequency / localization
	    >> Reference normal output ($000x)
	a1.l = Link chain table start address

	* Explanation about the link chain table is omitted.

Ret:	d0.l = 0	Success

----------------------------------------
Operation mode change   Func code $007x
----------------------------------------

Func:	Change the volume / frequency / pan of the active channel.

Call:	d0.w = $0070 + Channel number(0〜7)
	d1.l = Volume / frequency / localization
	    >> Reference normal output ($000x)

Ret:	d0.l = 0	Success

----------------------------------------
Data length query        Func code $008x
----------------------------------------

Func:	Get the remaining data length of the active channel.

Call:	d0.w = $0080 + Channel number(0〜7)

Ret:	d0.l = 0	Channel is stopped
	d0.l > 0	Indicates the remaining data length (byte) during normal output in the multiple reproduction mode.
	d0.l = -1	Array chain outputting
	d0.l = -2	Link chain outputting
	d0.l = -3	In mono tone reproduction mode, normal output

----------------------------------------
Operational mode query   Func code $009x
----------------------------------------

Func:	Get the volume / frequency / localization of the active channel.

Call:	d0.w = $0090 + Channel number(0〜7)

Ret:	d0.l = Volume / frequency / localization
	    >> Reference normal output ($000x)
	    Localization always takes values 1 to 3.
	    Bits 31 to 24 are 0.

----------------------------------------
End                      Func code $0100
----------------------------------------

Func:	The chain operation of all the channels is canceled and the channel operation is stopped as soon as the output of the currently output data block is completed.

	If it is paused, immediately stop the operation of all channels.

Call:	d0.w = $0100

Ret:	d0.l = 0	Success

Note:	Note that it is not an immediate termination.
	If you want to terminate completely immediately, issue this call after pause ($ 0101).
	During single tone IOCS output, this call is ignored.

----------------------------------------
Pause                    Func code $0101
----------------------------------------

Func:	The output of all channels is immediately stopped, and the pause state is started.

	When channel output or termination is performed in the suspended state, the operation of all channels is aborted.

Call:	d0.w = $0101

Ret:	d0.l = 0	Success

Note:	During single tone IOCS output, this call is ignored.

----------------------------------------
Unpause  		Func code $0102
----------------------------------------

Func:	一時停止状態を解除する。

	打ち切りの後は無効。

Call:	d0.w = $0102

Ret:	d0.l = 0	Success

Note:	単音IOCS出力中は、このコールは無視される。

----------------------------------------
MPU / MFP mask settings  Func code $01FB
----------------------------------------

Func:	ＰＣＭ８割込中のＭＰＵ/ＭＦＰ割込マスクを設定する。

Call:	d0.w = $01FB
	d1.l = Func/マスク値
	    < 0 : マスク取得のみ
	    >=0 : マスク設定
		bit 18〜16 : MPU  マスク
		bit 15〜 8 : IMRA マスク
		bit  7〜 0 : IMRB マスク

Ret:	d0.l = (変更前の)マスク値
		bit 31〜19 は 0 となる。

Note:	一般のプログラムでは使ってはならない。

	既定値は %011_11011111_00000000 。
	マスクされていない割込処理内からは、ファンクションコールを行ってはならない。

	内容を十分理解してから使用すること。

----------------------------------------
Multiple / single tone mode setting	Func code $01FC
----------------------------------------

Func:	Enable / disable multiple playback.

Call:	d0.w = $01FC
	d1.w = Authorization flag
	    -1 : Acquisition only
	     0 : 禁止
	     1 : 許可
	     2 : ファンクションコールのみ許可

Ret:	d0.l = 実際の許可状態
	     0 : 禁止中
	     1 : 許可中
	     2 : ファンクションコールのみ許可中

Note:	It should not be used in general programs.

	While multiple playback is prohibited, the output by the function is effective afterward. Furthermore, the IOCS output has priority over the function output.

	This call is independent of the permission / prohibition from the PCM 8 command line, and only when both settings are "permitted" it is set to the multiple reproduction mode.

	The return value reflects both settings.

----------------------------------------
(reserved)		Func code $01FD
----------------------------------------

	本コールは予約されています。

----------------------------------------
Occupancy			Func code $01FE
----------------------------------------

Func:	ＰＣＭ８を占有し、常駐解除を禁止する。

Call:	d0.w = $01FE

Ret:	d0.l = 0	占有した
	d0.l = -1	既に占有されている

Note:	一般のプログラムでは使ってはならない。

----------------------------------------
占有解除		Func code $01FF
----------------------------------------

Func:	ＰＣＭ８の占有を解除し、常駐解除を可能にする。

Call:	d0.w = $01FF

Ret:	d0.l = 0	占有解除した
	d0.l = -1	占有されていない

Note:	一般のプログラムでは使ってはならない。

----------------------------------------
常駐解除		Func code $FFFF
----------------------------------------

Func:	ＰＣＭ８の常駐を解除する。

Call:	d0.w = $FFFF

Ret:	d0.l = 0	常駐解除が実行された
	d0.l = -1	占有されており、解除出来なかった
	d0.l = -2	PCM8で使用している割込ベクタ等が変更されており、解
			除出来なかった

Note:	一般のプログラムでは使ってはならない。

	There are 7 vectors used in PCM8.

	VECTOR=	ADDR===	Content==========
	$022	$000088	TRAP #2
	$06A	$0001A8	DMAC ch.3 Interrupt
	$06B	$0001AC	DMAC ch.3 Error interrupt
	$160	$000580	IOCS _ADPCMOUT
	$162	$000588	IOCS _ADPCMAOT
	$164	$000590	IOCS _ADPCMLOT
	$167	$00059C	IOCS _ADPCMMOD


-- ＩＯＣＳ Call ( TRAP #15 )-------------------------------------------------


	Apart from the PCM 8 function call, IOCS calls are prepared so that it can be used in almost the same procedure as before.

　Supported call:

	$60 : _ADPCMOUT		チェインなし出力
	$62 : _ADPCMAOT		アレイチェイン出力
	$64 : _ADPCMLOT		リンクアレイチェイン出力
	$67 : _ADPCMMOD		ＡＤＰＣＭ実行制御

	上記以外のADPCM関連コール($61,$63,$65,$66)は、常駐前のままです。

　Difference from original IOCS:

　　（Multiple playback mode）

　　　・When outputting again within the sound being emitted, it synthesizes with the output of the other channels without waiting for output completion.

	In this case, use unused channels arbitrarily. The direction to search is from back to front (channels 8 · 7 · · · 1).
	When there is no vacancy, the channel with the shortest remaining time among the channels without output is crushed and output.
	Ignore the call if there is no corresponding channel.

　　　・With a $60 call, we return immediately even if the length is over $FF00.

　　　・パンは、最新の指定に従う。

	先に出力中の残響も影響を受ける。但し、出力カットの指定は完全に無視。

　　　・$67コールの終了指定は、チェイン動作の終了となる。

	現在出力中のデータブロックはそのまま出力される。即時終了する場合は、中止指定を行うこと。再開も可能。

	中止指定の後、終了指定または他の音の再生指定を行うと、元の出力は全てキャンセルされる。

　　（単音再生モード）

　　　・前の音が鳴っている内に再度出力を行った場合、出力終了を待たず、前の音の出力を取り消して出力する。

　　　・$60コールで、長さが $FF00 以上でもすぐリターンする。

　その他:

　　　・IOCS コールでは、16bit PCM , 8bit PCM の出力は出来ません。
　　　・IOCS コールの音量は 8(原音) 固定です。


-- How to determine the resident status of PCM8 ------------------------------------


	ＰＣＭ８ファンクションコールを使用する場合は、予め常駐判定を行っておく必要があります。

	常駐判定には、以下の方式を推奨します。

　方法１: トラップベクタ取得方式

	例外ベクタ$22 ( = ($0088).l ) の内容 ( = TRAP #2 エントリ ) の手前
	８バイトを検査する。

	先頭から５バイトが 'PCM4/' または 'PCM8/' ならば常駐している。その
	後の３バイトはバージョン。 ex) version 0.45 = 'PCM8/045'

	この判定方法は version 0.11 以上の PCM4/8 に対し有効です。

	PCM4 か PCM8 かを確実に判定したい場合は、バージョンで検査して下さい
	( 030 未満は PCM4 )。なお、version 0.40 以降のヘッダは 'PCM8/???'の
	形式に統一されています。

　方法２: ＩＯＣＳコール方式(簡略方式)

	d1.l に 定数 'PCM8' を設定して、IOCS $67(_ADPCMMOD) を発行。戻り値
	d0.l > 0 であれば PCM8 は常駐しており、バージョンは d0.l/100(10進)
	である。

	この判定方法は version 0.45 以上の PCM8 に対し有効です。


-- その他、特記事項 -----------------------------------------------------------


　・OPM レジスタアクセスについて

	PCM8 は、OPM レジスタをアクセスする場合があります。

	アクセスレジスタは $1B で、システムワーク $09DA を参照して値を決定
	しますので、他のアプリケーションではシステムワークと実際の設定内容
	の喰い違いが起こらないようにする必要があります。

　・PCM8 組み込み制御について

	音源ドライバが独自に ADPCM 処理を行う場合など、後から PCM8 が組み
	込まれると都合が悪い場合は、TRAP #2 のエントリ ( = ($0088).l ) に
	ダミーのアドレスを設定しておくと、PCM8 の組み込みを阻止する事が出
	来ます。

　・PCM8 ヘッダの位置について

	version 0.48 現在、PCM8 の識別ヘッダはロードアドレス先頭にあります
	が、これは将来変更する予定がありますので、例えばヘッダアドレスから
	MCB/PSP を逆算するようなコーディングは避けるようにしてください。


-------------------------------------------------------------------------------
```
