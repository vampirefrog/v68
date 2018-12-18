```
	- TRAP EXCEPTION MANAGER MANUAL -

==============================================================================

・TRAP 例外処理一覧

trap	#0	ユーザ定義(Ko-Window)
trap	#1	〃	  (bgdrv, mpcm)
trap	#2	〃	  (pcm8*)
trap	#3	〃	  (zmusic, zmsc3, middrv)
trap	#4	〃	  (mxdrv, madrv, mld, mcdrv, mndrv)
trap	#5	〃	  (cdc)
trap	#6	〃
trap	#7	〃

trap	#8	ブレークポイント(ROM デバッガ)
trap	#9	〃		(db、scd)
trap	#10	リセット／電原オフ処理
trap	#11	BREAK キー処理
trap	#12	COPY キー処理
trap	#13	CTRL+C 処理
trap	#14	エラー処理
trap	#15	IOCS call

※ ether_ne.sys では trap #0〜#6 のうちいずれか一つを使用する(使用しない場合も
   ある). また、inetd・hinetd・xip では trap #0〜#7 のうちいずれか一つを使用す
   る(使用しない場合もある).

==============================================================================

・trap #10	リセット／電原オフ処理

引数	d0.l =	$58363801('X68'<<8+1)	フロントスイッチによる電源オフ
		$58363802('X68'<<8+2)	外部スイッチによる電源オフ
		$58363803('X68'<<8+3)	ソフトスイッチによる電源オフ
		上記以外		ソフトウェアリセット

	  電源オフ割り込みとキーボード処理内、若しくはユーザプログラムから呼び
	出され、電源切断及びソフトウェアによるリセット処理を行う.
	  Human68k 起動時にはフックされて、DOS コール実行中に呼び出されてもす
	ぐには実行されず、終了してから呼び出されるようになる.

==============================================================================

・trap #11	BREAK キー処理

引数	d0.b	シフトキーの状態

	  BREAK キーが押された時に、キーボード処理から呼び出される. ただし、
	$40〜$4f のディスク関連 IOCS を実行中は無視される.
	  Human68k 起動時にはフックされて、DOS コール実行中に呼び出されてもす
	ぐには実行されず、終了してから呼び出されるようになる.
	  SHIFT を押さずに BREAK キーを押した場合は、trap #11 の後に trap #13
	も実行される.
	  多重呼び出しは行われないので、リエントラントな構造である必要はない.

==============================================================================

・trap #12	COPY キー処理

引数	d0.b	シフトキーの状態

	  COPY キーが押された時にキーボード処理から呼び出され、画面のハードコ
	ピー処理を行う.
	  多重呼び出しは行われないので、リエントラントな構造である必要はない.

	  シフトキー状態によって以下のような動作をする.

	COPY		縮小コピー(1色)
	SHIFT + COPY	拡大コピー(1色)
	CTRL  + COPY	拡大コピー(4色)
	OPT.1 + COPY	FF 出力.
	OPT.2 + COPY	LF 〃

==============================================================================

・trap #13	CTRL+C 処理

	  CTRL+C 及び BREAK キーが入力された時にキーボード処理から呼び出される.
	通常は何もしないルーチンが登録されているので、ユーザが使用できる.

==============================================================================

・trap #14	エラー処理

引数	d7.w	エラー番号
	a5.l	文字列のアドレス(d7.w = $??00 の場合のみ)
返値	d7.b	1:再実行 2:無視

	  起動直後はキー入力を待ってソフトウェアリセットによる再起動を行うだけ
	であるが、Human68k 起動後はフックされて、白帯を表示してユーザの応答を
	待つルーチンに置き換わる.

	  システムがエラーを検出した場合、d7.w にエラー番号をセットして trap
	#14 を実行する. このエラー処理ルーチンでは、エラーの種別に応じてメッセ
	ージを出力した後、キーボードからの入力によって、d7.b に 1(再実行)か 2
	(無視) をセットして復帰する. 中止が指定された場合はすぐにアボートし、
	trap #14 を実行したところには戻らない.

・エラー番号

$00xx		CPU 例外処理／未登録の割り込み(中止のみ)
		下位バイトがベクタ番号.
	$0002	バスエラーが発生した
	$0003	アドレスエラーが発生した
	$0004	おかしな命令を実行した
	$0005	0 で除算した
	$0006	CHK 命令を実行した
	$0007	TRAPV 命令を実行した
	$0008	特権命令を実行した
	$001f	インタラプトスイッチが押された
$01xx		未登録の IOCS コール
		下位バイトが IOCS コール番号.
		FD にしか実行できないコールを SASI に対して使用した時、若しく
		はその逆を行った時にも呼び出される.
$02xx〜$0fxx	予約
$10xx〜$1fxx	エラー(中止のみ)
$20xx〜$2fxx	エラー(再実行のみ)
$30xx〜$3fxx	エラー(再実行＆中止)
	$301f	インタラプトスイッチが押された
$40xx〜$4fxx	エラー(無視のみ)
$50xx〜$5fxx	エラー(無視＆中止)
$60xx〜$6fxx	エラー(無視＆再実行)
$70xx〜$7fxx	エラー(無視＆再実行＆中止)
$80xx〜$efxx	予約
$f0xx〜$fdxx	F 系列の DOS コール以外の割り込み(中止のみ)
$fexx		浮動小数点演算パッケージが組み込まれていない(中止のみ)
$ffxx		未登録の DOS コール
		Human68k version 3.0x では、未登録の DOS コールは -1 を返すだ
		けなのでエラー処理は呼び出されない.

	  インタラプトスイッチが押された場合のエラー番号は $001f と $003f の二
	通りあるが、通常は後者を使用する.

	  $1000〜$7fff のエラー番号は、上位バイトの bit 4、5、6 がそれぞれ中止、
	再実行、無視に対応する. この時の下位バイトの内容は以下の通り.

$00	ユーザ定義(a5.l に文字列のアドレスを入れる).
$01	無効なユニット番号を指定した.
$02	ディスクが入っていない.
$03	デバイスドライバに無効なコマンドを指定した.
$04	CRC エラー.
$05	ディスクの管理領域が破壊されている.
$06	シークエラー.
$07	無効なメディア.
$08	セクタが見つからない.
$09	プリンタが繋がっていない.
$0a	書き込みエラー.
$0b	読み込みエラー.
$0c	その他のエラー.
$0d	ライトプロテクト(プロテクトを外して同じディスクを入れる).
$0e	書き込み不可能.
$0f	ファイル共有違反.

	  下位バイトが 0 の場合、表示する文字列をユーザが指定する事が出来る.
	文字列のアドレスは a5.l で渡し、文字列は表示幅 52 桁以下であること. た
	だしデバイスドライバのエラーコードとして返す場合は下位バイト(リクエス
	トヘッダ先頭から 3 バイト目)が 0 であれば正常終了として扱われ、文字列
	のアドレスを渡すことも不可能なので、この機能は使用出来ない.

==============================================================================

・trap #15	IOCS processing

Arg	d0.b	IOCS call number

	  Execute IOCS.
	  During execution, the IOCS call number is stored in one word from IOCS work $a0e (upper byte is always 0).

	  Arguments and return values other than d0.b differ depending on the call number

	  IOCS コールを高速化するには、以下のような方法でショートカット呼び出しを実行する.
	  ただし、スーパーバイザ状態でなければならない.

	movea.l	(IOCSコール番号*4+$400),a0
	jsr	(a0)

	  IOCS の各機能は rts で帰るサブルーチンとして作成する.
	  a0.l は破壊しても問題ないが(ユーザから見た場合、ショートカットする時は保存される保証はない)、他の返値を渡さないレジスタについては破壊しないこと.

ベクタベースレジスタ vbr への対応方法:

	  X68030(ROM IOCS version 1.3)では、一部のIOCS コールで直接 ROM 内のル
	ーチンを呼び出しているものを除き、vbr を使用する事が出来る.

	  まず、以下のベクタを、ROM 内を指す標準のベクタであることを確認した上
	で、それぞれの設定アドレスから 2 を引いた値に変更する. 次に、それらの
	うち未使用の割り込み(処理アドレスの最上位バイトが 0 でないもの)の処理
	アドレスを $00ff0770 に変更する.
	  当然 vbr の指し示す先に正しいアドレステーブルを作成しておくこと.

	trap #15(IOCS 処理アドレステーブルも vbr+$400 に変更される)
	IOCS _FNTGET
	IOCS _OPMINTST
	IOCS _TIMERDST
	IOCS _VDISPST
	IOCS _CRTCRAS
	IOCS _HSYNCST
	IOCS _PRNINTST
	IOCS _B_INTVCS

==============================================================================
```