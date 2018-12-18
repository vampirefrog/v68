	- DOS CALL MANUAL -

==============================================================================

・DOS コール一覧

```
$ff00   _EXIT       Program end
$ff01   _GETCHAR    Keyboard input (with echo)
$ff02   _PUTCHAR    Character display
$ff03   _COMINP     RS-232C line 1 byte input
$ff04   _COMOUT     RS-232C line 1 byte output
$ff05   _PRNOUT     Printer 1 character output
$ff06   _INPOUT     Character input / output
$ff07   _INKEY      Keyboard one character input (no break check)
$ff08   _GETC       Keyboard one character input (with break check)
$ff09   _PRINT      String indication
$ff0a   _GETS       Character string input (with break check)
$ff0b   _KEYSNS     Key input state check
$ff0c   _KFLUSH     Keyboard input after buffer flush
$ff0d   _FFLUSH     Disk reset
$ff0e   _CHGDRV     Current drive setting
$ff0f   _DRVCTRL    Drive status check / setting

$ff10   _CONSNS     Screen output inspection
$ff11   _PRNSNS     Printer output inspection
$ff12   _CINSNS     RS-232C line input inspection
$ff13   _COUTSNS    RS-232C line output inspection
$ff17   _FATCHK     File connection state inspection
$ff18   _HENDSP     Kanji conversion line control
$ff19   _CURDRV     Current drive acquisition
$ff1a   _GETSS      Character string input (no break check)
$ff1b   _FGETC      File 1 byte input
$ff1c   _FGETS      File string input
$ff1d   _FPUTC      1 character file output
$ff1e   _FPUTS      File character string output
$ff1f   _ALLCLOSE   Close all files

$ff20   _SUPER      Supervisor / user mode setting
$ff21   _FNCKEY     Redefinable key acquisition / setting
$ff22   _KNJCTRL    Function call for kana-to-kanji conversion
$ff23   _CONCTRL    Console control / direct output
$ff24   _KEYCTRL    Console state check / direct input
$ff25   _INTVCS     Vector processing address setting
$ff26   _PSPSET     Create process management pointer
$ff27   _GETTIM2    Time acquisition (long word)
$ff28   _SETTIM2    Time setting (long word)
$ff29   _NAMESTS    File name expansion
$ff2a   _GETDATE    Date acquisition
$ff2b   _SETDATE    Date setting
$ff2c   _GETTIME    Time acquisition
$ff2d   _SETTIME    Time setting
$ff2e   _VERIFY     Verify flag setting
$ff2f   _DUP0       File handle forced copy

$ff30   _VERNUM     OS version acquisition
$ff31   _KEEPPR     End resident
$ff32   _GETDPB     Drive parameter block copy
$ff33   _BREAKCK    Break check setting
$ff34   _DRVXCHG    Drive replacement
$ff35   _INTVCG     Vectored address acquisition
$ff36   _DSKFRE     Disk remaining amount acquisition
$ff37   _NAMECK     File name expansion
$ff39   _MKDIR      Create subdirectory
$ff3a   _RMDIR      Remove subdirectory
$ff3b   _CHDIR      Change current directory
$ff3c   _CREATE     File creation
$ff3d   _OPEN       File open
$ff3e   _CLOSE      File close
$ff3f   _READ       File input

$ff40   _WRITE      File output
$ff41   _DELETE     Delete file
$ff42   _SEEK       Move file pointer
$ff43   _CHMOD      File attribute acquisition / setting
$ff44   _IOCTRL     Device driver ioctrl Direct I/O
$ff45   _DUP        File handle copy
$ff46   _DUP2       File handle forced copy
$ff47   _CURDIR     Acquire the current directory
$ff48   _MALLOC     Memory allocation
$ff49   _MFREE      Memory block release
$ff4a   _SETBLOCK   Memory block change
$ff4b   _EXEC       Program Load / Execute
$ff4c   _EXIT2      End code specification end
$ff4d   _WAIT       Process end code acquisition
$ff4e   _FILES      File search
$ff4f   _NFILES     File Next Search

$ff80   _SETPDB     Management process change
$ff81   _GETPDB     Process information acquisition
$ff82   _SETENV     Environment variable setting
$ff83   _GETENV     Acquisition of environmental variables
$ff84   _VERIFYG    Acquisition of verify flag
$ff85   _COMMON     common area control
$ff86   _RENAME     File name change / move
$ff87   _FILEDATE   File change date acquisition / setting
$ff88   _MALLOC2    Memory allocation
$ff8a   _MAKETMP    Create temporary file
$ff8b   _NEWFILE    File creation
$ff8c   _LOCK       File lock
$ff8f   _ASSIGN     Virtual drive / directory assignment acquired / set / canceled

$ffaa   _FFLUSH_SET Setting of FFLUSH mode
$ffab   _OS_PATCH   OS internal processing switching (system specific call)
$ffac   _GETFCB     FCB Pointer Acquisition
$ffad   _S_MALLOC   Secure memory block under main memory management
$ffae   _S_MFREE    Memory block under main memory management released
$ffaf   _S_PROCESS  Sub memory management setting

$fff0   _EXITVC     (Program execution end address)
$fff1   _CTRLVC     (CTRL + C execution address at abort)
$fff2   _ERRJVC     (Execution address at error abort)
$fff3   _DISKRED    Block device direct input
$fff4   _DISKWRT    Block device direct output
$fff5   _INDOSFLG   OS work pointer acquisition
$fff6   _SUPER_JSR  Subroutine call
$fff7   _BUS_ERR    Bus error occurrence inspection
$fff8   _OPEN_PR    Create background task
$fff9   _KILL_PR    Self process removal
$fffa   _GET_PR     Acquisition of thread management information
$fffb   _SUSPEND_PR Thread forced sleep
$fffc   _SLEEP_PR   Sleep
$fffd   _SEND_PR    Thread command / data transmission
$fffe   _TIME_PR    Timer counter value acquisition
$ffff   _CHANGE_PR  Abandonment of execution rights

```

```
注意	Human68k verion 2 で $ff50〜$ff7f に配置されていたコールは、verion 3
	以降では $ff80〜$ffaf に移動された.

	$fff5〜$fff7,$fffa〜$fffe のコールの処理アドレスは変更不可(無視される).
	$fff8,$ffff は本来の処理が正常終了した後に、設定したアドレスが実行され
	る. $fff9 は一つのスレッドを削除するごとに呼び出される.
```

```
==============================================================================

・Error code一覧

$ffffffff	 -1	I executed an invalid function code
$fffffffe	 -2	The specified file can not be found.
$fffffffd	 -3	The specified directory can not be found.
$fffffffc	 -4	Too many open files
$fffffffb	 -5	Directory and volume label can not be accessed
$fffffffa	 -6	The specified handle is not opened
$fffffff9	 -7	Memory management area was destroyed
$fffffff8	 -8	There is no memory required for execution
$fffffff7	 -9	Invalid memory management pointer specified
$fffffff6	-10	I specified an illegal environment
$fffffff5	-11	Execution file format is abnormal
$fffffff4	-12	Open access mode is abnormal
$fffffff3	-13	Invalid file name specification
$fffffff2	-14	Called with invalid parameters
$fffffff1	-15	Invalid drive specification
$fffffff0	-16	The current directory can not be deleted
$ffffffef	-17	Devices that can not be ioctrl
$ffffffee	-18	No more files found
$ffffffed	-19	Specified file can not be written
$ffffffec	-20	The specified directory is already registered
$ffffffeb	-21	I can not delete it because I have a file
$ffffffea	-22	I can not rename because there is a file
$ffffffe9	-23	Disk full and file can not be created
$ffffffe8	-24	The directory is full and I can not create a file
$ffffffe7	-25	You can not seek to the specified position
$ffffffe6	-26	I designated the supervisor mode again in the supervisor state
$ffffffe5	-27	The same thread name exists
$ffffffe4	-28	When the interprocess communication buffer is in a write-protected state
$ffffffe3	-29	I can not start any more background processes
$ffffffe0	-32	Lock space is insufficient
$ffffffdf	-33	It is locked and can not be accessed
$ffffffde	-34	The specified drive has a handler open
$ffffffdd	-35	Symbolic link nesting exceeded 16 times (lndrv)
$ffffffb0	-80	File exists

==============================================================================
```

```
==============================================================================

$ff00	_EXIT		プログラムの終了

	プログラムを終了する. オープンしたファイルハンドルはクローズされ、使用
	中のメモリブロックは全て解放される.
	終了コードは 0 となる.

==============================================================================

$ff01	_GETCHAR	キーボードの入力を待つ(エコーあり)

Ret	キーコード

	キーの入力を待ち、入力したキーコードを標準出力に出力する.
	入力されたキーが ^C(親プロセスに戻る)、^P(以後の標準出力をプリンタにも
	出力する)、^N(^P 状態をやめる)のいずれであるか調べる.

==============================================================================

$ff02	_PUTCHAR	指定文字コードの表示

Arg	CODE.w		1 バイト文字コード

Ret	常に 0

	CODE で指定した文字コードを標準出力に出力する.
	表示中に入力されたキーが ^C、^S、^P、^N であるか調べる.

	move	CODE,-(sp)
	DOS	_PUTCHAR
	addq.l	#2,sp

==============================================================================

$ff03	_COMINP		RS-232C 回線から 1 バイト入力

Ret	入力した 1 バイト

	RS-232C 回線から 1 バイト入力する(ブレークチェックあり).

==============================================================================

$ff04	_COMOUT		RS-232C 回線へ 1 バイト出力

Arg	CODE.w		1 バイトコード

	CODE で指定した 1 バイトのコードを RS-232C 回線に出力する(ブレークチェ
	ックあり).

	move	CODE,-(sp)
	DOS	_COMOUT
	addq.l	#2,sp

==============================================================================

$ff05	_PRNOUT		プリンタへ 1 文字出力

Arg	CODE.w		1 バイト文字コード

	CODE で指定した 1 文字をプリンタへ出力する(ブレークチェックあり).
	CODE の上位バイトは 0 でなければならないので、漢字は上位・下位の順で 1
	バイトずつ出力する.

	move	CODE,-(sp)
	DOS	_PRNOUT
	addq.l	#2,sp

==============================================================================

$ff06	_INPOUT		キーの入出力

Arg	CODE.w		1 バイト文字コード

Ret	CODE = $ff,$fe
		入力されたキーコード(d0.l = 0 なら入力なし)
	CODE = その他
		d0.l = 0

	CODE の値が $ff であればキー入力を行うが、キーが入力されていなければす
	ぐに終了する. $fe であればキーの先読み入力を行うが、キーが入力されてい
	てもそのキーはそのままバッファに残る. CODE がその他の値であれば文字コ
	ードとみなして表示する.

	move	CODE,-(sp)
	DOS	_INPOUT
	addq.l	#2,sp

==============================================================================

$ff07	_INKEY		キーボードから 1 文字入力(ブレークチェックなし)

Ret	キーコード

	キーが押されるまで待ち、そのキーコードを返す.

==============================================================================

$ff08	_GETC		キーボードから 1 文字入力(ブレークチェックあり)

Ret	キーコード

	キーが押されるまで待ち、そのキーコードを返す. ^C、^P、^N について調べ
	る.

==============================================================================

$ff09	_PRINT		文字列の表示

Arg	MESPTR.l	文字列のポインタ

Ret	Always 0

	MESPTR で指定した文字列を表示する. ^C、^S、^P、^N について調べる.

	pea	(MESPTR)
	DOS	_PRINT
	addq,l	#4,sp

==============================================================================

$ff0a	_GETS		文字列の入力

Arg	BUFFER.l	入力バッファのポインタ

Ret	入力文字数

	BUFFER で指定した入力バッファに、改行までの入力文字列を書き込む.
	改行文字はヌル文字に置き換えられる. ^C、^P、^N について調べる.
	最大入力文字数を超えた場合は、警告を出すが終了はしない.
	入力バッファの内容は以下の通りで、最大入力文字数を設定してから呼び出す.
	同一バッファで続けて実行する場合、前回の実行時に得られた入力文字数をク
	リアしなければテンプレート機能が使用できる.

offset	size
0	1.b	最大入力文字数 : n
1	1.b	実際に入力された文字数
2	n+1.b	入力文字列

	pea	(BUFFER)
	DOS	_GETS
	addq.l	#4,sp

==============================================================================

$ff0b	_KEYSNS		キーの入力状態の検査

Ret	入力状態(0:入力なし、-1:入力あり)

	キーの入力状態を調べる. ^C、^P、^N について調べる.

==============================================================================

$ff0c	_KFLUSH		バッファフラッシュ後キーボードから入力

Arg	MODE.w		モード
	CODE.w		処理を撰択するコード
	BUFFER.l	入力バッファのポインタ

Ret	MODE = 1,7,8
			入力されたキーコード
	MODE = 6
		CODE = $fe,$ff
			入力されたキーコード(0 なら入力なし)
		CODE = その他
			d0.l = 0
	MODE = 10
			入力文字数

	キー入力バッファを空にしてから、キーボードから入力する.
	MODE の値は DOS コールの下位 1 バイトに対応し、以下の動作をする.

	MODE = 1
		キー入力を待ち、入力されたコードを標準出力に出力する.
	MODE = 6
		CODE = $ff	キー入力(入力がなくても待たない)を調べる.
		CODE = $fe	キーの先読み入力を行う.
		CODE = その他	標準出力に出力する.
	MODE = 7
		キー入力を待つ(ブレークチェックなし).
	MODE = 8
		キー入力を待つ(ブレークチェックあり).
	MODE = 10
		改行までに入力した文字列をバッファに書き込む.

MODE = 1,7,8
	move	MODE,-(sp)
	DOS	_KFLUSH
	adddq.l	#2,sp

MODE = 6
	move	CODE,-(sp)
	move	MODE,-(sp)
	DOS	_KFLUSH
	adddq.l	#4,sp

MODE = 10
	pea	(BUFFER)
	move	MODE,-(sp)
	DOS	_KFLUSH
	adddq.l	#6,sp

==============================================================================

$ff0d	_FFLUSH		ディスクのリセット

Ret	常に 0

	ディスクバッファの内容と標準入力からの行入力バッファを初期化する. ファ
	イルのクローズはしない. fflush = off であれば何もしない.

==============================================================================

$ff0e	_CHGDRV		カレントドライブの設定

Arg	DRIVE.w		ドライブ番号(0:A 1:B … 25:Z)

Ret	指定可能ドライブ数(1〜26)

	カレントドライブを変更する. Retが指定したドライブ番号以下であれば失敗
	したことになる.

	move	DRIVE,-(sp)
	DOS	_CHGDRV
	addq.l	#2,sp

==============================================================================

$ff0f	_DRVCTRL	ドライブの状態検査/設定

Arg	MODE.w		(MD.b×256＋DRIVE.b)
		MB.b		モード
		DRIVE.b		ドライブ番号(0:カレント 1:A 2:B … 26:Z)
	P1.l		Arg 1
	P2.l		Arg 2
	…
	Pn.l		Arg n

Ret	MD = 0
		指定ドライブの状態

			bit 7	LED 点滅
			bit 6	イジェクト禁止
			bit 5	バッファあり
			bit 4	ユーザーによるイジェクト禁止
			bit 3	PRO(プロテクト = 1)
			bit 2	RDY(ノットレディ = 1)
			bit 1	メディア挿入
			bit 0	誤挿入

		注: PRO と RDY はメディアが挿入されていた場合のみ返す.

	モード MD の値に従って、ドライブ番号 DRIVE で指定したドライブの状態を
	検査/設定する.
	MODE の上位 8bit にモード MD、下位 8bit にドライブ番号 DRIVE を指定す
	る.

	MD = 0		状態検査.
	MD = 1		排出(オープン中のファイルがあるとエラー).
	MD = 2		排出禁止(MD = 1 の排出も禁止).
	MD = 3		排出許可(オープンされているドライブはクローズされず、
			バッファは自動的に消去される).
	MD = 4		ディスク未挿入時に LED 点滅.
	MD = 5〜8	使用禁止.
	MD = 9		カレントディレクトリをルートにして検索 FAT を先頭に戻
			す(オープンされているファイルがあるとエラー).
	MD = 10		検索 FAT を先頭に戻す.
	MD = 11〜15	予約.
	MD = 16〜	特殊ブロックデバイスに対して使用する(任意のArgが追加
			可能).

MD = 0〜4,9,10
	move	MODE,-(sp)
	DOS	_DRVCTRL
	addq.l	#2,sp

MD = 16〜
	move.l	Pn,-(sp)
	…
	move.l	P1,-(sp)
	move	MODE,-(sp)
	DOS	_DRVCTRL
	lea	(n*4+2,sp),sp

==============================================================================

$ff10	_CONSNS		画面出力ができるかの検査

Ret	出力状態(0:出力不可、0 以外:出力可能)

	画面に出力可能か調べる.

==============================================================================

$ff11	_PRNSNS		Check whether printer output is possible

Ret	Output status (0: not possible、non-zero: output possible)

	Check whether output to the printer is possible.

==============================================================================

$ff12	_CINSNS		RS-232C 回線から入力ができるかの検査

Ret	入力状態(0:入力不可、0 以外:入力可能)

	RS-232C 回線から入力可能か調べる.

==============================================================================

$ff13	_COUTSNS	RS-232C 回線へ出力ができるかの検査

Ret	出力状態(0:出力不可、0 以外:出力可能)

	RS-232C 回線に出力可能か調べる.

==============================================================================

$ff17	_FATCHK		ドライブのセクタが連続しているかの検査

Arg	FILE.l		ファイル名のポインタ
	BUFFER.l	バッファのポインタ
	LEN.w		バッファのバイト数

Ret	バッファの使用バイト数(負数ならError code)
		d0.l =  8	連続([1]でコールした場合)
		d0.l = 14	連続([2]でコールした場合)

	FILE で指定したファイルのドライブ番号とセクタの連結状態をバッファに書
	き込む. この DOS コールで FAT が連続していることを調べるとともに、
	DISKRED でデータ部を直接読むことが可能.
	この DOS コールは、バッファのアドレス BUFFER をスタックに積む時
	[1] そのまま積む
	[2] BUFFER＋$80000000 を積む
	の二通りのコール方法があり、[1]の場合はバッファに書き込まれるセクタ番
	号とセクタ数のサイズがワードであるが、[2]ではロングワードになる.
	[2]の場合バッファのバイト数 LEN がArgに必要なので注意すること.
	バッファに書き込まれる情報は以下の通り.

[1]
offset	size
0	1.w	ドライブ番号	: d(1:A 2:B … 26:Z)
2	1.w	先頭セクタ番号	: s1
4	1.w	セクタ数	: n1
6	1.w	次のセクタ番号	: s2
8	1.w	セクタ数	: n2
	…
?	1.w	終わり		: 0

[2]
offset	size
0	1.w	ドライブ番号	: d(1:A 2:B … 26:Z)
2	1.l	先頭セクタ番号	: s1
6	1.l	セクタ数	: n1
10	1.l	次のセクタ番号	: s2
14	1.l	セクタ数	: n2
	…
?	1.l	終わり		: 0

[1]
	pea	(BUFFER)
	pea	(FILE)
	DOS	_FATCHK
	addq.l	#8,sp

[2]
	move	LEN,-(sp)
	pea	(BUFFER+$80000000)
	pea	(FILE)
	DOS	_FATCHK
	lea	(10,sp),sp

==============================================================================

$ff18	_HENDSP		漢字変換行の制御

Arg	MD.w		コントロールモード
	POS.w		ポジション(変換ウィンドウの先頭からの位置)
	MESPTR.l	変換文字列のポインタ

Ret	MD = 0,4,8
		ウィンドウの最大文字数
	MD = 1,2,5,6,7,10
		次のポジション

	モード MD の値に従って漢字変換行を制御する.
	この DOS コールは漢字変換を扱うプログラムに使用するが、日本語 FP から
	利用するので一般のアプリケーションは使用禁止.

	MD = 0
		モード表示ウィンドウをオープンする.
	MD = 1
		指定位置から文字列をノーマルで表示する.
	MD = 2
		指定位置から文字列をリバースで表示する.
	MD = 3
		モード表示ウィンドウをクローズする.
	MD = 4
		入力ウィンドウをオープンする.
	MD = 5
		指定位置から文字列をノーマルで表示する.
	MD = 6
		指定位置から文字列をリバースで表示する.
	MD = 7
		指定のポジション以降を元に戻す.
	MD = 8
		侯補ウィンドウをオープンする.
	MD = 9
		指定位置から文字列をノーマルで表示する.
	MD = 10
		指定位置から文字列をリバースで表示する.
	MD = 11
		侯補ウィンドウをクローズする.

MD = 0,3,4,8,11
	move	MD,-(sp)
	DOS	_HENDSP
	addq.l	#2,sp

MD = 7
	move	POS,-(sp)
	move	MD,-(sp)
	DOS	_HENDSP
	addq.l	#4,sp

MD = 1,2,5,6,9,10
	pea	(MESPTR)
	move	POS,-(sp)
	move	MD,-(sp)
	DOS	_HENDSP
	addq.l	#8,sp

==============================================================================

$ff19	_CURDRV		カレントドライブを得る

Ret	カレントドライブの番号(0:A 1:B … 25:Z)

	カレントドライブの値を調べる.

==============================================================================

$ff1a	_GETSS		文字列の入力

Arg	BUFFER.l	入力バッファのポインタ

Ret	入力文字数

	BUFFER で指定した入力バッファに、改行までの入力文字列を書き込む.
	改行文字は、ヌル文字に置き換えられる.
	ブレークチェックはしない(VOID/NEWLINE で改行しない).
	最大入力文字数を超えた場合は、警告を出すが終了はしない.
	入力バッファの内容は以下の通り.

offset	size
0	1.b	最大入力文字数	: n
1	1.b	(実際に入力された文字数)
2	n+1.b	入力文字列

	pea	(BUFFER)
	DOS	_GETSS
	addq.l	#4,sp

==============================================================================

$ff1b	_FGETC		1 byte input from the file handle

Arg	FILENO.w	File handle

Ret	Input 1 byte code

	Wait until there is input from the file handle specified by FILENO, and return the code if there is input.

==============================================================================

$ff1c	_FGETS		ファイルハンドルから文字列入力

Arg	BUFFER.l	入力バッファのポインタ
	FILENO.w	ファイルハンドル

Ret	入力文字数

	FILENO で指定したファイルハンドルから改行までの文字列を入力し、BUFFER
	で指定した入力バッファに書き込む. 改行文字はヌル文字に置き換えられる.
	ブレークチェックはしない(VOID/NEWLINE で改行しない).
	最大入力文字数を超えた場合は、最大入力文字数まで入れて終了する.
	入力バッファの内容は以下の通り.

offset	size
0	1.b	最大入力文字数	: n
1	1.b	(実際に入力された文字数)
2	n+1.b	入力文字列

	move	FILENO,-(sp)
	pea	(BUFFER)
	DOS	_FGETS
	addq.l	#6,sp

==============================================================================

$ff1d	_FPUTC		ファイルハンドルに 1 バイト出力

Arg	CODE.w		1 バイトコード
	FILENO.w	ファイルハンドル

	CODE で指定した 1 バイトコードを、FILENO で指定したファイルハンドルに
	出力する.

	move	FILENO,-(sp)
	move	CODE,-(sp)
	DOS	_FPUTC
	addq.l	#4,sp

==============================================================================

$ff1e	_FPUTS		Write string to file handle

Arg	MESPTR.l	String pointer
	FILENO.w	File handle

	MESPTR で指定した文字列を、FILENO で指定したファイルハンドルに出力する.
	文字列の末尾のヌル文字は出力しない.

	move	FILENO,-(sp)
	pea	(MESPTR)
	DOS	_FPUTS
	addq.l	#6,sp

==============================================================================

$ff1f	_ALLCLOSE	全ファイルのクローズ

	オープンしたファイルを全てクローズする.

==============================================================================

$ff20	_SUPER		Switch between supervisor and user mode

Arg STACK.l		切り替えモード(0 若しくは SSP に設定するアドレス)

Ret	STACK = 0
		前の SSP の値(負数ならError code)

	切り替えモード STACK の値に従って、スーパーバイザ/ユーザーモードを切り
	替える.

	STACK = 0
		USP の値を SSP に設定し、スーパーバイザモードに切り替える.
	STACK = その他
		STACK を SSP に設定し、ユーザーモードに切り替える.

	pea	(STACK)
	DOS	_SUPER
	addq.l	#4,sp

==============================================================================

$ff21	_FNCKEY		再定義可能なキーの読み出し/設定

Arg	MODE.w		モード及びキー番号(MD.b×256＋FNO.b)
	BUFFER.l	データバッファのポインタ

Ret	d0.l =	0	正常終了
		-1	キー番号が異常

	モード MD の値によって、再定義可能なキーの収得/設定をする.
	この DOS コールは、CON デバイス(コンソール)でサポートされる.
	MODE の上位 8bit にモード MD、下位 8bit にキー番号 FNO を指定する.

	MD = 0	収得
	MD = 1	設定(同時にファンクションキーの表示も行なうが、32 行モードの時
		は表示しない)

	FNO	キー名
	 0	全てのキー
	 1〜10	F01〜F10
	11〜20	SHIFT＋F01〜F10
	21	ROLL UP
	22	ROLL DOWN
	23	INS
	24	DEL
	25	UP(↑)
	26	LEFT(←)
	27	RIGHT(→)
	28	DOWN(↓)
	29	CLR
	30	HELP
	31	HOME
	32	UNDO

	BUFFER は収得/設定のためのバッファで、大きさは FNO の値によって異なる.

	FNO =  0	712 バイト(32×20＋6×12 バイト)
	FNO =  1〜20	 32 バイト(31 バイト＋$00)
	FNO = 21〜32	  6 バイト( 5 バイト＋$00)

	pea	(BUFFER)
	move	MODE,-(sp)
	DOS	_FNCKEY
	addq.l	#6,sp

==============================================================================

$ff22	_KNJCTRL	仮名漢字変換用ファンクションコールの呼び出し

	この DOS コールは、日本語 FP のファンクションコールで使用する.

==============================================================================

$ff23	_CONCTRL	Console control / direct output

Arg	MD.w		Control mode (args are different depending on this value)
	CODE.w		1 byte code
	MESPTR.l	String pointer
	ATR.w		Text attribute
	X.w			Cursor position
	Y.w			Cursor position
	N.w			Specify cursor relative movement
	MOD.w		Screen control designation
	YS.w		Scroll range specification
	YL.w		Scroll range specification

Ret	MD = 0〜1
		Cursor position
		d0.hw	Column
		d0.w	Row
	MD = 2
		変更前の文字属性(-1 ならエラー)
	MD = 3
		変更前のカーソル位置(-1 ならエラー)
		d0.hw	桁位置
		d0.w	行〃
	MD = 4〜5、7〜9、12〜13
		常に 0
	MD = 6
		d0.l =  0	正常終了
		d0.l = -1	エラー(移動できない)
	MD = 10、17〜18
		不定
	MD = 11
		d0.l =  0	正常終了
		d0.l = -1	エラー(モードが不正若しくはカーソル位置が不正)
				ROM IOCS version 1.3 未満で後者の原因でエラー
				になった場合、-1 以外の負数になることがある.
	MD = 14、16
		変更前のモード
	MD = 15
		d0.l =  0	正常終了
		d0.l = -1	エラー(範囲指定が不正)

	コントロールコード MD の値に従って、コンソールから直接入力/制御を行う.
	この DOS コールは CON デバイスでサポートされる.

	MD = 0
		Display 1 byte code specified by CODE (IOCS _B_PUTC).
	MD = 1
		Display the character string specified by MESPTR (IOCS _B_PRINT).
	MD = 2
		ATR で指定した文字属性を設定する(IOCS _B_COLOR). ATR = -1 の時
		は現在の文字属性を収得する.
	MD = 3
		カーソルを X、Y で指定した位置に設定する(IOCS _B_LOCATE). X =
		-1 の時は現在のカーソル位置を収得する.
	MD = 4
		カーソルを 1 行下に移動する. 最下行でスクロールアップする
		(IOCS _B_DOWN_S).
	MD = 5
		カーソルを 1 行上に移動する. 先頭行でスクロールダウンする
		(IOCS _B_UP_S).
	MD = 6
		カーソルを N 行上に移動する. 移動できない時はエラーになる
		(IOCS _B_UP).
	MD = 7
		カーソルを N 行下に移動する. 移動できない時は最下行まで移動す
		る(IOCS _B_DOWN).
	MD = 8
		カーソルを N 桁右に移動する. 移動できない時は右端まで移動する
		(IOCS _B_RIGHT).
	MD = 9
		カーソルを N 桁左に移動する. 移動できない時は左端まで移動する
		(IOCS _B_LEFT).
	MD = 10
		モード MOD の値に従って、画面を消去する(IOCS _B_CLR_ST).

		MOD = 0		カーソル位置から最終行右端まで
		MOD = 1		先頭行左端からカーソル位置まで
		MOD = 2		画面全体(カーソルは先頭行左端に移動する)
	MD = 11
		モード MOD の値に従って、現在行を消去する(IOCS _ERA_ST).

		MOD = 0		カーソル位置から行の右端まで
		MOD = 1		行の左端からカーソル位置まで
		MOD = 2		カーソル行全体
	MD = 12
		カーソル行に N 行挿入する(IOCS _B_INS).
	MD = 13
		カーソル行から N 行削除する(IOCS _B_DEL).
	MD = 14
		モード MOD の値に従って、ファンクションキー行のモードを設定し、
		前のモードを返す. スクロール範囲はリセットされる.

		MOD =  0	ファンクションキーを表示(スクロール範囲は 0 か
				ら 31 行分)
		MOD =  1	シフトファンクションキーを表示(同上)
		MOD =  2	何も表示しない(同上)
		MOD =  3	何も表示しない(スクロール範囲は 0 から 32 行分)
		MOD = -1	現在のモードを調べる
	MD = 15
		YS、YL でスクロール範囲を設定する.
		スクロール後、カーソルは先頭行左端に移動する.
		YS＋YL の値は、ファンクションキー行のモードが 3 の時は 32 まで、
		それ以外は 31 までの範囲で指定する.
		絶対座標の (0,YS) が 論理座標の (0,0) となる.
	MD = 16
		モード MOD に従って画面モードを設定し、前のモードを返す.
		IOCS _TGUSEMD でグラフィック画面を使用中に設定していても、MOD
		= 1、3〜5 がエラーにならずに画面モードを変更してしまうので注意
		すること.

		MOD	表示画面	グラフィック
		0	768×512	    なし
		1	768×512	   16 色
		2	512×512	    なし
		3	512×512	   16 色
		4	512×512	  256 色
		5	512×512	65536 色
		-1	現在のモードを調べる.
	MD = 17
		カーソルを表示するモードにする(IOCS _OS_CURON).
	MD = 18
		カーソルを表示しないモードにする(IOCS _OS_CUROF).

	MD = 6〜9、12〜13 で N = 0 を指定した場合は N = 1 として扱われる.

MD = 0
	move	CODE,-(sp)
	move	MD,-(sp)
	DOS	_CONCTRL
	addq.l	#4,sp

MD = 1
	pea	(MESPTR)
	move	MD,-(sp)
	DOS	_CONCTRL
	addq.l	#6,sp

MD = 2
	move	ATR,-(sp)
	move	MD,-(sp)
	DOS	_CONCTRL
	addq.l	#4,sp

MD = 3
	move	Y,-(sp)
	move	X,-(sp)
	move	MD,-(sp)
	DOS	_CONCTRL
	addq.l	#6,sp

MD = 4,5,17,18
	move	MD,-(sp)
	DOS	_CONCTRL
	addq.l	#2,sp

MD = 6〜9,12〜13
	move	N,-(sp)
	move	MD,-(sp)
	DOS	_CONCTRL
	addq.l	#4,sp

MD = 10,11,14,16
	move	MOD,-(sp)
	move	MD,-(sp)
	DOS	_CONCTRL
	addq.l	#4,sp

MD = 15
	move	YL,-(sp)
	move	YS,-(sp)
	move	MD,-(sp)
	DOS	_CONCTRL
	addq.l	#6,sp

==============================================================================

$ff24	_KEYCTRL	コンソールの状態検査/直接入力

Arg	MD.w		コントロールモード(この値によってArgが異なる)
	GROUP.w		キーコードグループ
	INSMODE.w	INS キーのモード(-1:ON 0:OFF)

Ret	MD = 0、1
		入力されたキーコード
	MD = 2
		シフトキーの状態(ビット 10〜4 は 1 の時 LED 点灯、ビット 3〜0
		は 1 の時キーが押されている)
			bit 10	全角
			bit  9	ひらがな
			bit  8	INS
			bit  7	CAPS
			bit  6	コード入力
			bit  5	ローマ字
			bit  4	かな
			bit  3	OPT.2
			bit  2	OPT.1
			bit  1	CTRL
			bit  0	SHIFT
	MD = 3
		キーの状態
	MD = 4
		常に 0

	コントロールコード MD の値に従って、直接コンソールから入力/状態検査を
	行う.
	この DOS コールは CON デバイスでサポートされる.

	MD = 0
		キーを入力する.
	MD = 1
		キーコードを先読みする.
	MD = 2
		シフトキーの状態を調べる.
	MD = 3
		GROUP で指定したキーコードグループのキー状態を調べる.
	MD = 4
		INS キーを ON/OFF する.

MD = 0〜2
	move	MD,-(sp)
	DOS	_KEYCTRL
	addq.l	#2,sp

MD = 3
	move	GROUP,-(sp)
	move	MD,-(sp)
	DOS	_KEYCTRL
	addq.l	#4,sp

MD = 4
	move	INSMODE,-(sp)
	move	MD,-(sp)
	DOS	_KEYCTRL
	addq.l	#4,sp

==============================================================================

$ff25	_INTVCS		Set processing address of vector

Arg	INTNO.w		Vector number
	JOBADR.l	Processing address of specified vector

Ret	The previous processing address

	Set JOBADR to the vector specified by vector number INTNO and return the previous setting address.

	INTNO = $0000〜$00ff  interrupt
	INTNO = $0100〜$01ff  IOCS call
	INTNO = $ff00〜$ffff  DOS call

	When addresses of trap # 10 to 14 and _EXITVC are changed, work in the PSP is also changed.
	In addition, _ERRJVC further changes sr and ssp at the time of abort to those at the moment when this DOS call was executed.
	In the case of _CTRLVC, after changing the address of _CTRLVC and the work in the PSP, the same processing as _ ERRJVC is performed.

	pea	(JOBADR)
	move	INTNO,-(sp)
	DOS	_INTVCS
	addq.l	#6,sp

==============================================================================

$ff26	_PSPSET		Create process management pointer

Arg	PSPADR.l	Address of process management pointer (memory management pointer + 16)

Ret	Always 0

	PSPADR で指定したアドレスに、プログラム終了時の様々な情報を作成する.
	プロセス管理ポインタ PSPADR はメモリ管理ポインタ＋$10 のアドレスでなくてはならず、その大きさは 240 バイト.

	pea	(PSPADR)
	DOS	_PSPSET
	addq.l	#4,sp

==============================================================================

$ff27	_GETTIM2	Get time (longword)

Ret	Current time

		%0000_0000_000t_tttt_00mm_mmmm_00ss_ssss
		bit 20〜16	Hour(0〜23)
		bit 13〜 8	Minute(0〜59)
		bit  5〜 0	Second(0〜59)

	Get the current time. The difference from DOS _GETTIME is that the return value is a longword because seconds is one seconds unit.

==============================================================================

$ff28	_SETTIM2	時刻の設定(ロングワード)

Arg	TIME.l		時刻(形式は DOS _GETTIM2 のRetと同じ)

Ret	Error code

	時刻を TIME の値に設定する. DOS _SETTIME との違いは、秒が 1 秒単位なの
	で TIME がロングワードになっていることである.

	move.l	TIME,-(sp)
	DOS	_SETTIM2
	addq.l	#4,sp

==============================================================================

$ff29	_NAMESTS	ファイル名の展開

Arg	FILE.l		ファイル名のポインタ
	BUFFER.l	バッファのポインタ

Ret	Error code

	FILE で指定したファイルを、BUFFER で指定したバッファに展開する.
	バッファは 88 バイトで、内容は以下の通り.

offset	size
0	1.b	NAMWLD	0:ワイルドカードなし -1:ファイル指定なし
			(ワイルドカードの文字数)
1	1.b	NAMDRV	ドライブ番号(A=0,B=1,…,Z=25)
2	65.b	NAMPTH	パス('\'＋あればサブディレクトリ名＋'\')
67	8.b	NAMNM1	ファイル名(先頭 8 文字)
75	3.b	NAMEXT	拡張子
78	10.b	NAMNM2	ファイル名(残りの 10 文字)

	pea	(BUFFER)
	pea	(FILE)
	DOS	_NAMESTS
	addq.l	#8,sp

==============================================================================

$ff2a	_GETDATE	Get the date

Ret	Current date

		%0000_0000_0000_0www_yyyy_yyym_mmmd_dddd
		bit 18〜16	Week day(0〜6)
		bit 15〜 9	Year-1980(0〜99)
		bit  8〜 5	Month (1〜12)
		bit  4〜 0	Day (1〜31)

	現在の日付を調べる. 曜日は 0〜6 が日〜土曜日を表し、年は西暦から 1980 を引いた値が 0〜99 まで設定されている.

==============================================================================

$ff2b	_SETDATE	日付の設定

Arg	DATE.w		日付

Ret	Error code

	DATE の値を日付に設定する. DATE の形式は以下の通りで、年は西暦から
	1980 を引いた値を 0〜99 まで設定する.

	%yyyy_yyym_mmmd_dddd
	bit 15〜 9	年-1980(0〜99)
	bit  8〜 5	月(1〜12 月)
	bit  4〜 0	日(1〜31 日)

	move	DATE,-(sp)
	DOS	_SETDATE
	addq.l	#2,sp

==============================================================================

$ff2c	_GETTIME	Get current time

Ret	Current time (meaning only for lower words)

		%tttt_tmmm_mmms_ssss
		bit 15〜11	時  (0〜23 時)
		bit 10〜 5	分  (0〜59 分)
		bit  4〜 0	秒/2(0〜29 秒)

	現在の時刻を調べる. Retのうち秒は 2 秒単位で表し、実際の秒を 2 で割っ
	た値になる. DOS _GETTIM2 との違いは、秒が 2 秒単位なのでRetがワードに
	なっていることである.

==============================================================================

$ff2d	_SETTIME	時刻の設定

Arg	TIME.w		時刻(形式は DOS _GETTIME のRetと同じ)

Ret	Error code

	時刻を TIME の値に設定する. このうち秒は 2 秒単位で表し、実際の秒を 2
	で割った値になる. したがって、設定する値は 0〜29 まで. DOS _SETTIM2 と
	の違いは、秒が 2 秒単位なので TIME がワードになっていることである.

	move	TIME,-(sp)
	DOS	_SETTIME
	addq.l	#2,sp

==============================================================================

$ff2e	_VERIFY		ベリファイフラグの設定

Arg	FLG.w		ベリファイフラグ(0:ベリファイしない 1:する)

	ベリファイフラグを設定する.

	move	FLG,-(sp)
	DOS	_VERIFY
	addq.l	#2,sp

==============================================================================

$ff2f	_DUP0		ファイルハンドルの強制複写

Arg	FILENO.w	ファイルハンドル
	NEWNO.w		複写先のファイルハンドル

Ret	NEWNO の前の値(負数ならError code)

	ファイルハンドル FILENO を、2 個目の ファイルハンドル NEWNO に強制複写
	する. これにより 1 個のファイルに 2 個のファイルハンドルが与えられ、同
	一ファイルの操作を複数のファイルハンドルで行うことが出来るようになる.
	クローズすると元の番号に戻る.
	指定できる NEWNO は 0〜4(標準ファイルハンドル)だけで、それより大きい数
	のハンドルの場合は DUP2 で指定する.

	move	NEWNO,-(sp)
	move	FILENO,-(sp)
	DOS	_DUP0
	addq.l	#4,sp

==============================================================================

$ff30	_VERNUM		OS version check

Ret	Version number

		bit 31〜24	'6'($36)
		bit 23〜16	'8'($38)
		bit 15〜 8	Integer part of version number
		bit  7〜 0	Decimal part of version number

	Determine the version number of Human68k.
	For version 3.02 it looks like d0.l = $36380302.

==============================================================================

$ff31	_KEEPPR		Terminate and stay resident

Arg	PRGLEN.l	Number of resident bytes
	CODE.w		Exit code

	Leave the PRGLEN byte (not including the PSP part) and end the process.
	If the specified number of bytes can not be secured, resident terminates with the size of the current memory block.
	Note that open file handles will not be closed and the memory blocks in use will not be released as well.
	The size actually resident in the memory is the specified resident byte number + the memory management pointer 16 bytes + the process management pointer 240 bytes.
	The exit code is the value ($0001_xxxx) where the upper word of the value specified by CODE becomes 1 (_EXIT and _EXIT2 is $0000_xxxx).

	move	CODE,-(sp)
	move.l	PRGLEN,-(sp)
	DOS	_KEEPPR

==============================================================================

$ff32	_GETDPB		ドライブパラメータブロックの複写

Arg	DRIVE.w		ドライブ番号(0:カレント 1:A 2:B … 26:Z)
	DPBPTR.l	バッファのポインタ

Ret	Error code

	DRIVE で指定したドライブのドライブパラメータブロックを、Human68k
	version 1 互換の形式に変換して、DPBPTR で指定した 94 バイトのバッファ
	に複写する.
	Human68k version 1 互換形式 DPB の内容は以下の通り.

offset	size
0	1.b	ドライブ番号(0:A 1:B … 25:Z)
1	1.b	デバイスドライバで使うユニット番号
2	1.w	1 セクタ当りのバイト数
4	1.b	1 クラスタ当りのセクタ数-1
5	1.b	クラスタ→セクタのシフト数
		bit 7 = 1 で MS-DOS 形式 FAT(16bit Intel 配列)
6	1.w	FAT の先頭セクタ番号
8	1.b	FAT 領域の個数
9	1.b	FAT の占めるセクタ数(複写分を除く)
10	1.w	ルートディレクトリに入るファイルの個数
12	1.w	データ領域の先頭セクタ番号
14	1.w	総クラスタ数+1
16	1.w	ルートディレクトリの先頭セクタ番号
18	1.l	デバイスドライバへのポインタ
22	1.b	メディアバイト
23	1.b	DPB 使用フラグ(-1 なら書き込みが行われた)
24	1.l	次の DPB へのポインタ(-1 で末尾)
28	1.w	カレントディレクトリのクラスタ番号(0 はルートを表す)
30	64.b	カレントディレクトリの文字バッファ

	pea	(DPBPTR)
	move	DRIVE,-(sp)
	DOS	_GETDPB
	addq.l	#6,sp

==============================================================================

$ff33	_BREAKCK	ブレークチェックの設定

Arg	FLG.w		機能及びモード(FUNC.b×256+MODE.b)
			FLG = -1 の場合は FUNC = 0 と同じ.

Ret	設定状況(MODE の 0〜2 と同じ)

	フラグ FLG の値に従って、ブレークチェックを設定する.
	Retは今設定した値そのものであるので注意すること.

	FUNC =	0	DOS コールでのブレークチェックを設定する
		MODE =	0	指定の DOS コールのみ検査する
			1	全ての DOS コールで検査する
			2	全ての DOS コールで検査しない
			-1	設定状況を調べる

	指定の DOS コールとは、ブレークチェックをする以下のコールを指す.

	$ff01	GETCHAR
	$ff02	PUTCHAR
	$ff03	COMINP
	$ff04	COMOUT
	$ff05	PRNOUT
	$ff08	GETC
	$ff09	PRINT
	$ff0a	GETS
	$ff0b	KEYSNS
	$ff0c	KFLUSH
	$ff0d	FFLUSH
	$ff1e	FPUTS

	Human68k version 3.00 以降では下記の機能も使用可能.

	FUNC =	1	CTRL+C のモードを設定する
	FUNC =	2	CTRL+N のモードを設定する
	FUNC =	3	CTRL+P のモードを設定する
	FUNC =	4	CTRL+Q のモードを設定する
	FUNC =	5	CTRL+S のモードを設定する
		それぞれ MODE の内容は以下の通り.
		MODE =	0	無効
			1	有効
			-1	設定状況を調べる

	move	#FLG,-(sp)
	DOS	_BREAKCK
	addq.l	#2,sp

==============================================================================

$ff34	_DRVXCHG	ドライブの入れ替え

Arg	OLD.w		ドライブ番号(0:カレント 1:A 2:B … 26:Z)
	NEW.w		ドライブ番号(〃)

Ret	OLD = -1、NEW = -1
		カレントドライブの新しいドライブ番号(0:A 1:B … 25:Z)
	その他
		Error code

	OLD で指定したドライブと、NEW で指定したドライブを入れ替える.
	OLD、NEW ともに -1 を指定すると、全てのドライブ変更を元に戻す.

	move	NEW,-(sp)
	move	OLD,-(sp)
	DOS	_DRVXCHG
	addq.l	#4,sp

==============================================================================

$ff35	_INTVCG		Get processing address of vector

Arg	INTNO.w		Vector number

Ret	Processing address of specified vector

	ベクタ番号 INTNO で指定したベクタの処理アドレスを調べる.

	INTNO = $0000〜$00ff	割り込み
	INTNO = $0100〜$01ff	IOCS コール
	INTNO = $ff00〜$ffff	 DOS コール

	move	INTNO,-(sp)
	DOS	_INTVCG
	addq.l	#2,sp

==============================================================================

$ff36	_DSKFRE		ディスクの残り容量を得る

Arg	DRIVE.w		ドライブ番号
	BUFFER.l	バッファのポインタ

Ret	使用可能なバイト数(最大 2GB、負数ならError code)

	DRIVE で指定したドライブのディスクの残り容量を調べ、結果を BUFFER で指
	定した 8 バイトのバッファに書き込む.

offset	size
0	1.w	使用可能なクラスタ数
2	1.w	総クラスタ数
4	1.w	1 クラスタ当りのセクタ数
6	1.w	1 セクタ当りのバイト数

	pea	(BUFFER)
	move	DRIVE,-(sp)
	DOS	_DSKFRE
	addq.l	#6,sp

==============================================================================

$ff37	_NAMECK		Expand file name

Arg	FILE.l		Pointer of file name
	BUFFER.l	Buffer pointer

Ret	d0.l = $ff	No file specification
	d0.l =   0	Wild card not specified
	d0.l <   0	Error code (contents of BUFFER have no meaning)
	d0.l = Other wildcard specified (d0.l is the number of wildcard characters)

	Expands the filename specified by FILE into the 91-byte buffer specified by BUFFER.

offset	size
0	  1+1.b	Drive name + ':'
2	 64+1.b	Path name + 0
67	 18+1.b	File name + 0
86	1+3+1.b	Extension('.'＋Extension＋0)

	pea	(BUFFER)
	pea	(FILE)
	DOS	_NAMECK
	addq.l	#8,sp

==============================================================================

$ff39	_MKDIR		サブディレクトリの作成

Arg	FILE.l		ディレクトリ名のポインタ

Ret	Error code

	FILE で指定したサブディレクトリを作成する.
	成功した場合のRetは通常 0 であるが、特殊デバイスドライバが対象の時は
	正の整数が返る場合がある.

	pea	(FILE)
	DOS	_MKDIR
	addq.l	#4,sp

==============================================================================

$ff3a	_RMDIR		サブディレクトリの削除

Arg	FILE.l		ディレクトリ名のポインタ

Ret	Error code

	FILE で指定したサブディレクトリを削除する. 指定するディレクトリは空で
	なくてはならず、またカレントディレクトリ・ルートディレクトリやシステム
	属性・読み込み専用属性のディレクトリは削除出来ない.
	成功した場合のRetは通常 0 であるが、特殊デバイスドライバが対象の時は
	正の整数が返る場合がある.

	pea	(FILE)
	DOS	_RMDIR
	addq.l	#4,sp

==============================================================================

$ff3b   _CHDIR      Change current directory

Arg  FILE.l      Pointer to directory name

Return value error code

Change the current directory to the directory specified by FILE.
In the case of success, the return value is usually 0, but when a special device driver is targeted, a positive integer may be returned.
It does not work properly with Human68k version 3.02 (change to a subdirectory of length 22 characters fails).

    pea (FILE)
    DOS _CHDIR
    addq.l  #4,sp

==============================================================================

$ff3c	_CREATE		Create a file

Arg	FILE.l		Pointer to filename
	ATR.w		File attribute(%XLAD_VSHR)

Ret	File handle of the created file (Error code if negative number)

	FILE で指定したファイルを ATR で指定した属性で作成し、オープンする.
	シェアリングモードは互換モードとして扱われる.
	同名ファイルが既に存在していた場合はそのファイルの内容は失われるが、それがディレクトリかボリュームラベル、またはシステム属性・読み込み専用属性のファイルであった場合には削除せずエラーになる.

	ATR が 0 の場合はアーカイブ属性(%0010_0000)と見なされる. また、Human68k version 3.00 以降で ATR の最上位ビットを立てると、同名ファイルが存在するかどうかの検索を行わず、常に新しいファイルを作成する.
	これは dirsch=on の設定をより高速化する為の機能であるが、既に存在するファイル名を指定した場合は同名のファイルが複数存在することになってしまうので注意すること.

	move	ATR,-(sp)
	pea	(FILE)
	DOS	_CREATE
	addq.l	#6,sp

==============================================================================

$ff3d	_OPEN		Open a file

Arg	FILE.l		Pointer to filename
	MODE.w		Access mode

Ret	File handle of open file (negative number if an error occurred)

	Open the file specified by FILE in the access mode specified by MODE.
	It is not possible to open a file with system/read-only attribute in write or read/write mode.
	THe contents of MODE are as follows:

	bit    8	Dictionary access mode
			%1	Use a special file handle for the dictionary (user prohibited).
			%0	Normal
	bit 6〜4	 Sharing mode
			%100	Always allowed
			%011	Allow write-only
			%010	Allow read-only
			%001	Always prohibited
			%000	Compatibility mode
	bit 1〜0	Read/write mode
			%10	Read / write mode
			%01	Write mode
			%00	Read mode

	The sharing mode specified by bit 6〜4 regulates the sharing of files by multiple processes. When multiple processes are executed in parallel, multiple processes can read and write to the same file at the same time There is a risk of doing so, it is necessary to designate an appropriate sharing mode and exclusive control of file access.

	In the dictionary access mode specified by bit 7, two files can be opened simultaneously. Since ASK68K ver.3.0 uses one ASK68K body, another can be used with an accessory.

	move	MODE,-(sp)
	pea	(FILE)
	DOS	_OPEN
	addq.l	#6,sp

==============================================================================

$ff3e	_CLOSE		Close the file

Arg	FILENO.w	File handle

Ret	Error code

	Close the file handle specified by FILENO.

	In Human68k version 3.00 or later, even when the handle copied with _DUP is closed, the file name and update date and time are written to the standard block device.

	move	FILENO,-(sp)
	DOS	_CLOSE
	addq.l	#2,sp

==============================================================================

$ff3f	_READ		ファイルから読み込む

Arg	FILENO.w	ファイルハンドル
	BUFFER.l	読み込みバッファのポインタ
	LEN.l		読み込むバイト数

Ret	実際に読み込んだバイト数(負数ならError code)

	FILENO で指定したファイルハンドルから、LEN で指定するバイト数を BUFFER
	で指定するバッファに読み込む. 実行後、ファイルポインタは読み込んだバイ
	ト数だけ後方に移動する.
	Retが指定したバイト数より小さければ、ファイルを終わりまで読み込んだこ
	とを意味する.

	move.l	LEN,-(sp)
	pea	(BUFFER)
	move	FILENO,-(sp)
	DOS	_READ
	lea	(10,sp),sp

==============================================================================

$ff40	_WRITE		Write to file

Arg	FILENO.w	File handle
	BUFFER.l	Pointer to buffer to write
	LEN.l		Number of bytes to write

Ret	The actual number of bytes written (Error code if negative number)

	FILENO で指定したファイルハンドルに、LEN で指定するバイト数を BUFFERで指定するバッファから書き込む.
	実行後、ファイルポインタは書き込んだバイト数だけ後方に移動する.
	LEN = 0 の場合は現在のファイルポインタ位置以降を切り捨て、ファイルサイズを縮小する.
	Retが指定したバイト数より小さい時は、ディスクの空き容量が足りなくなったことを意味する.

	move.l	LEN,-(sp)
	pea	(BUFFER)
	move	FILENO,-(sp)
	DOS	_WRITE
	lea	(10,sp),sp

==============================================================================

$ff41	_DELETE		ファイルの削除

Arg	FILE.l		ファイル名のポインタ

Ret	Error code

	FILE で指定したファイルを削除する.
	ワイルドカードの指定や、ディレクトリかボリュームラベル、またはシステム属性・読み込み専用属性のファイルの削除は出来ない.

	pea	(FILE)
	DOS	_DELETE
	addq.l	#4,sp

==============================================================================

$ff42	_SEEK		Move file position

Arg	FILENO.w	File handle
	OFFSET.l	Offset
	MODE.w		Move mode

Ret	先頭からのオフセット(負数ならError code)

	FILENO で指定したファイルのポインタを、MODE で指定した位置から OFFSETで指定したバイト数だけ移動する.
	ポインタをファイル終端以降若しくは先頭より前方に移動するとエラーになる.
	キャラクタデバイスはシーク出来ず、現在位置は常に 0 が返る.

	MODE = 0	Lead
	MODE = 1	Current position
	MODE = 2	終端(オフセットは 0 若しくは負数のみ指定可能)

	move	MODE,-(sp)
	move.l	OFFSET,-(sp)
	move	FILENO,-(sp)
	DOS	_SEEK
	addq.l	#8,sp

==============================================================================

$ff43	_CHMOD		ファイル属性の変更

Arg	FILE.l		ファイル名のポインタ
	ATR.w		ファイル属性(%XLAD_VSHR)

Ret	指定したファイルの属性(負数ならError code)

	FILE で指定したファイルを ATR で指定した属性に変更する.
	ATR = -1 の時はファイルの属性を調べる.

	move	ATR,-(sp)
	pea	(FILE)
	DOS	_CHMOD
	addq.l	#6,sp

==============================================================================

$ff44	_IOCTRL		Direct I/O by device driver ioctrl

Arg	MD.w		Control mode (Arg is different depending on this value)
	FILENO.w	File handle
	DT.w		Device intelligence
	DRIVE.w		Equipment number
	PTR.l		Pointer of buffer etc.
	LEN.l		Number of bytes to read/write
	COUNT.w		Number of retries
	TIME.w		Retry wait time
	F_CODE.w	Function number

Ret	MD = 0,1,10
		Device info
		(The lower byte is acquired from FCB and the upper byte is obtained from the device header)

		bit 15	0:Block device                     1:Character device
		bit 14	0:IOCTRL is not available          1:Available
		bit 13	0:local                            1:remote
		bit  7	0:Block device                     1:Character device
		bit  6	0:Special IOCTRL is not available  1:Available
		(For character devices)
		bit  5	0:COOKED mode                      1:RAW mode
		bit  3	1:CLOCK device
		bit  2	1: NUL  〃
		bit  1	1:Standard output (CON) device
		bit  0	1:Standard input (CON) 〃
		(In case of block device)
		bit  5	0:local                            1:remote
		bit 4〜0 Drive number

	MD = 9
		装置情報

		bit 15	0:ブロックデバイス	 1:キャラクタデバイス
		bit 14	0:IOCTRL 不可		 1:可能
		bit 13	0:ローカル		 1:リモート
		bit  5	0:ローカル		 1:リモート (bit 13 と同じ)

	MD = 2〜5
		実際に読み書きしたバイト数
	MD = 6
		入力ステータス(0:不可 -1:可)
	MD = 7
		出力ステータス(0:不可 -1:可)
	MD = 11,12,13
		デバイスドライバによる.

	モード MD の値に従って、デバイスドライバに直接入出力する. ただし、デバ
	イスヘッダの IOCTRL ビットが 0 の場合は、入出力出来ない.

	MD = 0
		Check device information of the file handle specified by FILENO.
	MD = 1
		Set the device information to the file handle specified by FILENO.
		Only the attribute of COOKED / RAW mode of character device chan be changed.
	MD = 2
		FILENO で指定したファイルハンドルから、PTR で指定したバッファ
		に LEN で指定したバイト数だけ読み込む.
	MD = 3
		FILENO で指定したファイルハンドルに、PTR で指定したバッファか
		ら LEN で指定したバイト数だけ書き込む.
	MD = 4
		DRIVE で指定したドライブから、PTR で指定したバッファに LEN で
		指定したバイト数だけ読み込む.
		リモートドライブからの入力は常にエラーが返る.
	MD = 5
		DRIVE で指定したドライブに、PTR で指定したバッファから LEN で
		指定したバイト数だけ書き込む.
		リモートドライブへの出力は常にエラーが返る.
	MD = 6
		FILENO で指定したファイルハンドルの入力ステータスを調べる.
		リモートファイルの検査は常に失敗する.
	MD = 7
		FILENO で指定したファイルハンドルの出力ステータスを調べる.
		リモートファイルの検査は常に失敗する.
	MD = 9
		DRIVE で指定したドライブがローカルかリモートかを調べる.
		実際には装置情報を調べているだけである.
	MD = 10
		Check whether the drive specified by FILE is local or remote.
		In fact it is just looking at device information.
	MD = 11
		指定した COUNT 及び TIME をリトライ数/待機時間に設定する.
	MD = 12
		ファイルハンドルによる特殊コントロールを行なう.
		Human68k version 3.02 では正常に動作しない(キャラクタデバイス
		に対して特殊 ioctrl が出来るかどうかの検査に失敗する).
	MD = 13
		ドライブによる特殊コントロールを行なう.

		Human68k version 3.00 以降の DISK2HD デバイスでは、2HD
		以外の FD に対応する為に以下のように拡張されている.
		F_CODE = -1 の場合、PTR に 'DISKdev1' の 8 バイトを返す.
		F_CODE = 0 の場合、PTR の先頭 1 ワードにメディアバイトを返す.
		F_CODE = 1 の場合、PTR には以下の情報を返す.

		offset	size
		0	1.l	trap #15 のRet(d0 の値 = エラービット)
		4	1.w	1 セクタ当りのバイト数(1024)
		6	1.b	FAT の占めるセクタ数(1)
		7	1.b	FAT 領域の個数(2)
		8	1.w	予約セクタ数(1)
		10	1.w	ルートディレクトリに入るファイルの個数(192)
		12	1.w	総セクタ数(8*77*2
		14	1.b	メディアバイト($fe)
		15	1.b	FAT エリアセクタ数(2)
		16	1.w	ヘッダ内セクタ数(8)
		18	1.w	トラック内セクタ数(2)
		20	1.b	IOCS 制御 d1.high パラメータ($90)
		21	1.b	メディアフラグワークエリア(-1)
				(メディアチェック用)

		F_CODE = 2 の場合、PTR はバッファアドレスではなく以下の値を指
		定する. 上記 PTR の最後の 1 バイトの値を変更する機能である.

		PTR = -1	排出されたものとして BPB と DPB を再構築する.
		PTR = 0		IOCS _B_DRVCHK のRetに従う(通常).
		PTR = 1		排出されていないものとしてメディアチェックを 1
				回無視する.

		なお、上記の他に特殊 ioctrl に対応しているデバイスドライバは、
		現在確認しているものでは susie.x がある.

MD = 0,6,7,10
	move	FILENO,-(sp)
	move	MD,-(sp)
	DOS	_IOCTRL
	addq.l	#4,sp

MD = 1
	move	DT,-(sp)
	move	FILENO,-(sp)
	move	MD,-(sp)
	DOS	_IOCTRL
	addq.l	#6,sp

MD = 2,3
	move.l	LEN,-(sp)
	pea	(PTR)
	move	FILENO,-(sp)
	move	MD,-(sp)
	DOS	_IOCTRL
	lea	(12,sp),sp

MD = 4,5
	move.l	LEN,-(sp)
	pea	(PTR)
	move	DRIVE,-(sp)
	move	MD,-(sp)
	DOS	_IOCTRL
	lea	(12,sp),sp

MD = 9
	move	DRIVE,-(sp)
	move	MD,-(sp)
	DOS	_IOCTRL
	addq.l	#4,sp

MD = 11
	move	TIME,-(sp)
	move	COUNT,-(sp)
	move	MD,-(sp)
	DOS	_IOCTRL
	addq.l	#6,sp

MD = 12,13
	pea	(PTR)
	move	F_CODE,-(sp)
	move	FILENO,-(sp)
	move	MD,-(sp)
	DOS	_IOCTRL
	lea	(10,sp),sp

==============================================================================

$ff45	_DUP		ファイルハンドルの複写

Arg	FILENO.w	ファイルハンドル

Ret	新しいファイルハンドル(負数ならError code)

	ファイルハンドル FILENO を複写して、2 個目のファイルハンドルを作る.
	同一ファイルのファイルハンドルは、同じファイルポインタを使用するのでどちらのファイルハンドルを使ってもファイルの操作が出来るようになる.

	move	FILENO,-(sp)
	DOS	_DUP
	addq.l	#2,sp

==============================================================================

$ff46	_DUP2		ファイルハンドルの強制複写

Arg	FILENO.w	ファイルハンドル
	NEWNO.w		複写先のファイルハンドル

Ret	Error code

	ファイルハンドル FILENO を 2 個目のファイルハンドル NEWNO に強制的に複写する.
	NEWNO で指定したファイルハンドルが既にオープンされている場合は、自動的にクローズしてから複写する.

	move	NEWNO,-(sp)
	move	FILENO,-(sp)
	DOS	_DUP2
	addq.l	#4,sp

==============================================================================

$ff47   _CURDIR     Get the current directory

Arg DRIVE.w Drive number (0: Current 1: A 2: B ... 26: Z)
    BUFFER.l buffer pointer

Return value Error code

Write the current directory of the drive specified by DRIVE
in the buffer of 65 bytes specified by BUFFER Drive name.
It does not attach '\' to the root directory and '\' to the end.

pea (BUFFER)
move DRIVE, -(sp)
DOS _CURDIR
addq.l # 6, sp

==============================================================================

$ff48	_MALLOC		Allocate memory

Arg	LEN.l		Number of bytes to allocate

Ret	Address of allocated memory block (MSP＋$10)
		d0.l = $81??????	指定した量は確保不可能
					($00?????? は確保可能なバイト数)
		d0.l = $8200000?	完全に確保不可能($? は不定)

	LEN で指定したバイト数のメモリを確保する. LEN が $1000000 以上の場合は
	必ずエラーになり、確保可能な最大バイト数を調べる事が出来る.

	move.l	LEN,-(sp)
	DOS	_MALLOC
	addq.l	#4,sp

==============================================================================

$ff49	_MFREE		Free memory block

Arg	MEMPTR.l	Pointer to memory block

Ret	Error code

	MEMPTR で指定したメモリブロックを解放する.
	MEMPTR = 0 の時、自分が確保したメモリを全て解放する.

	pea	(MEMPTR)
	DOS	_MFREE
	addq.l	#4,sp

==============================================================================

$ff4a	_SETBLOCK	Change memory block

Arg	MEMPTR.l	Pointer to memory block
	LEN.l		Number of bytes

Ret	Error code
		d0.l = $81??????	変更不可能
					($00?????? は変更可能なバイト数)
		d0.l = $8200000?	完全に変更不可能($? は不定)

	MEMPTR で指定したメモリブロックを、LEN で指定した大きさに変更する.
	LEN が $1000000 以上の場合は必ずエラーになり、変更可能な最大バイト数を調べる事が出来る.
	プロセス自体のメモリブロックを指定する時は LEN に PSP のサイズも含めなければならない(メモリ管理ポインタのサイズは除く).
	よって、DOS _KEEPPRで指定するサイズと同じ大きさに変更したい場合は、_KEEPPR で指定する値より 240 大きい値にする.

	move.l	LEN,-(sp)
	pea	(MEMPTR)
	DOS	_SETBLOCK
	addq.l	#8,sp

==============================================================================

$ff4b	_EXEC		プログラムのロード/実行

Arg	MD.w		モジュール番号及びモード(MODULE.b×256+MODE.w)
	FILE.l		ファイル名のポインタ
	CMDLINE.l	コマンドラインのポインタ
	LOADADR.l	ロードアドレス
	EXECADR.l	実行アドレス
	FILE2.l		オーバーレイ X ファイル中のファイル名
	ENVPTR.l	環境のポインタ
	LIMIT.l		リミットアドレス
	TYPE.b		ファイルタイプ(FILE の上位 8bit)

Ret	MODE = 0,4
		プロセス終了コード(負数ならError code)
		プロセスが終了した時 d1-d7/a1-a6 は不定.
	MODE = 1
		d0.l	プログラムの実行アドレス(負数ならError code)
		a0.l	メモリ管理ポインタ
		a1.l	プログラムの終わり
		a2.l	コマンドライン
		a3.l	環境のポインタ
		a4.l	実行アドレス
	MODE = 2
		Error code
	MODE = 3
		プログラムの長さ(負数ならError code)
	MODE = 5
		モジュール番号×256(負数ならError code)

	モード MODE の値に従って FILE で指定したファイルをロード/実行する.

	MODE = 0
		CMDLINE でコマンドラインを、ENVPTR で環境を指定して、FILE で指
		定したファイルをロードし、実行する.
	MODE = 1
		CMDLINE でコマンドラインを、ENVPTR で環境を指定して、FILE で指
		定したファイルをロードする. 正常終了した場合は必ず MODE = 4 を
		実行すること.
	MODE = 2
		ENVPTR で指定した環境から path を検索して、FILE で指定したコマ
		ンド行をフルパスのファイル名とコマンドラインに分け、FILE と
		CMDLINE の各ポインタに設定する.
		ファイル名バッファ FILE は 90 バイト以上、コマンドラインバッフ
		ァ CMDLINE は 256 バイト以上必要.
	MODE = 3
		LOADADR でロードアドレスを、LIMIT でリミットアドレスを指定して、
		FILE で指定したファイルをロードする.
	MODE = 4
		EXECADR で指定したアドレスからプログラムを実行する.
		MD = 1 でロードした後、実行する時に使用する.
	MODE = 5
		FILE で指定したオーバーレイ X ファイル中の、FILE2 で指定したフ
		ァイルのモジュール番号を調べる.
		バインドリスト先頭からのオフセットが $fffff00 より大きいファイ
		ルは、オフセット値をビット反転した数をモジュール番号として返す.
		ただし、このようなオフセット値が負数のファイルはロード出来ない.
		Human68k version 3.02 では正常に動作しない(FILE2 で指定したフ
		ァイル名の拡張子 1 バイト目を X に変更したファイル名で検索され
		る).

	MODE = 0〜2 の時、ENVPTR に 0 を指定した場合は自分の環境を使用する.
	MODE = 0,1,3 の時、MODULE でオーバーレイ X ファイル中の各ファイルのモ
	ジュール番号(0〜255)を指定することが可能. また、実行ファイル名の拡張子
	が .X .Z .R のいずれでもない場合は、FILE の上位 8bit でファイルタイプ
	TYPE を指定し、それによってファイルの形式を指定する.

	TYPE = 1	.R
	TYPE = 2	.Z
	TYPE = 3	.X

MD = 0,1
	pea	(ENVPTR)
	pea	(CMDLINE)
	pea	(FILE)
	move.b	TYPE,(sp)
	move	MD,-(sp)
	DOS	_EXEC
	lea	(14,sp),sp

MD = 2
	pea	(ENVPTR)
	pea	(CMDLINE)
	pea	(FILE)
	move	MD,-(sp)
	DOS	_EXEC
	lea	(14,sp),sp

MD = 3
	pea	(LIMIT)
	pea	(LOADADR)
	pea	(FILE)
	move.b	TYPE,(sp)
	move	MD,-(sp)
	DOS	_EXEC
	lea	(14,sp),sp

MD = 4
	pea	(EXECADR)
	move	MD,-(sp)
	DOS	_EXEC
	addq.l	#6,sp

MD = 5
	pea	(FILE2)
	pea	(FILE)
	move	MD,-(sp)
	DOS	_EXEC
	lea	(10,sp),sp

==============================================================================

$ff4c	_EXIT2		Specify exit code and exit

Arg	CODE.w		Exit code

	Quit the program with the exit code specified by CODE.
	The opened filehandle is closed and all the memory blocks in use are released.

==============================================================================

$ff4d	_WAIT		プロセスの終了コードを得る

Ret	終了コード

	実行したプロセスの終了コードを調べる.
	DOS _EXEC(MODE = 0,4) のRetと同じ.

==============================================================================

$ff4e	_FILES		ファイルを検索し、情報を得る(最初のファイル)

Arg	BUFFER.l	バッファのポインタ
	FILE.l		ファイル名のポインタ
	ATR.w		ファイル属性(%XLAD_VSHR)

Ret	Error code

	ATR で指定した属性で、FILE で指定したファイル名にあてはまるファイルを
	検索し、ファイル情報を BUFFER で指定した 53 バイトのバッファに書き込む.
	ファイル名にはワイルドカードが使用可能.
	属性は 1 を指定したビットが 1 個でも一致すれば、そのファイルを返す.

	ファイル属性の上位バイトを 1 にすると、ファイル名若しくは拡張子がない
	場合にワイルドカード '*' で補完する("" の検索は "*.*" になり、"foo" の
	検索は "foo.*" として扱われる).

	Human68k version 3.00 以降ではバッファアドレスの最上位ビットを 1 にす
	ると、通常の情報に加えて検索するファイルのパス名とファイル名をバッファ
	の 53 バイト目からの 88 バイトに書き込む. この場合のバッファは 141 バ
	イト必要である. 書き込まれる内容は、検索するファイル名を DOS _NAMESTS
	で展開したものとほぼ同じで、先頭の 2 バイトが "A:" 形式のドライブ名に
	なっている. これらの情報は DOS _FILES を呼び出した時に一度だけセットさ
	れる(DOS _NFILES ではセットされない). また、ファイル名は検索に成功した
	名称ではなく、'?' を含む検索対象のファイル名である. Human68k version
	3.02 では正常に動作しない(DOS _DRVXCHG でドライブを交換している場合は
	絶対ドライブ名で返る).

offset	size
0	1.b	検索する属性				┐
1	1.b	〃	ドライブ番号			│
2	1.l	検索中のディレクトリのセクタ番号	│Human68k 内部で
6	1.w	ディレクトリの残りセクタ数		│使用する.
		(ルートの場合のみ有効)			│破壊すると
8	1.w	ディレクトリ上のセクタ先頭からの	│_NFILES 出来ない.
		オフセット(-1 の時該当ファイルなし)	│
10	8.b	検索するファイル名			│
18	3.b	〃	拡張子				┘
21	1.b	属性
22	1.w	最終変更時刻
24	1.w	最終変更月日
26	1.l	ファイルサイズ
30	23.b	ファイル名(主ファイル名＋'.'＋拡張子＋0)

	バッファアドレスの最上位ビットを 1 にした場合、以下の情報が加わる.

offset	size
53	 2.b	ドライブ名＋':'
55	65.b	パス名('\'＋あればサブディレクトリ名＋'\')
120	 8.b	ファイル名(先頭 8 バイト)
128	 3.b	拡張子(3 バイト)
131	10.b	ファイル名(残りの 10 バイト)

	なお、TwentyOne 常駐時には内部情報の一部が変更され、TwentyOne 内部のフ
	ァイル名バッファ EXBUF が不足した場合はError code -8 を返す.

offset	size
0〜9	10.b	(標準と同じ)
10	1.l	拡張バッファのアドレス
14	6.b	TwentyOne 拡張フラグ 'Twenty'
20	1.b	エラーフラグ(0:OK 1:ERR)
21	32.b	(標準と同じ)

	move	ATR,-(sp)
	pea	(FIL)
	pea	(BUFFER)
	DOS	_FILES
	lea	(10,sp),sp

==============================================================================

$ff4f	_NFILES		次のファイルを検索し、情報を得る

Arg	BUFFER.l	バッファのポインタ

Ret	Error code

	_FILES で設定されたファイル情報バッファ BUFFER を使用し、次のファイル
	を検索してファイル情報を再度書き込む. BUFFER の内容は _FILES と同じ.

	pea	(BUFFER)
	DOS	_NFILES
	addq.l	#4,sp

==============================================================================

$ff80	_SETPDB		管理プロセスを移す

Arg	PDBADR.l	プロセス管理ポインタ

Ret	以前のプロセス管理ポインタ

	PDBADR で指定したプロセスに管理を移す.
	PDBADR は _GETPDB で得られたアドレスでなければならない.

	pea	(PDBADR)
	DOS	_SETPDB
	addq.l	#4,sp

==============================================================================

$ff81	_GETPDB		Get current process information

Ret	Current process management pointer (PSP)

	A process management pointer representing the current process is obtained.

==============================================================================

$ff82	_SETENV		環境変数の設定

Arg	ENVNAME.l	環境変数名のポインタ
	ENVPTR.l	環境のポインタ
	ENVVAL.l	変数の値のポインタ

Ret	Error code

	ENVPTR で指定した環境の ENVNAME で指定した変数に、ENVVAL で指定した最
	大 255 バイトの環境変数を設定する. ただし、ENVVAL = 0 の時や ENVVAL に
	空文字列を指定した場合は、ENVNAME で指定した変数を消去する.
	ENVPTR に 0 を指定すると自分の環境を使用する.
	Human68k version 3.02 では正常に動作しない(環境変数の値が 255 バイトを
	超えている時に暴走する).

	pea	(ENVVAL)
	pea	(ENVPTR)
	pea	(ENVNAME)
	DOS	_SETENV
	lea	(12,sp),sp

==============================================================================

$ff83	_GETENV		環境変数の内容を得る

Arg	ENVNAME.l	環境変数名のポインタ
	ENVPTR.l	環境のポインタ
	BUFFER.l	バッファのポインタ

Ret	Error code

	ENVPTR で指定した環境の、ENVNAME で指定した環境変数の内容を BUFFER で
	指定した 256 バイトのバッファに書き込む.
	ENVPTR に 0 を指定すると自分の環境を使用する.

	pea	(BUFFER)
	pea	(ENVPTR)
	pea	(ENVNAME)
	DOS	_GETENV
	lea	(12,sp),sp

==============================================================================

$ff84	_VERIFYG	ベリファイフラグを得る

Ret	設定状況(0:ベリファイしない 1:ベリファイする)

	ベリファイフラグを調べる.

==============================================================================

$ff85	_COMMON		common 領域の制御

Arg	MD.w		コントロールモード
	NAME.l		common 領域名のポインタ
	POS.l		ポジション(common 領域の先頭からのバイト数)
	BUFFER.l	バッファのポインタ
	ID_PSP.l	プロセス ID
	LEN.l		バイト長

Ret	MD = 0
		common 領域のバイト数(負数ならError code)
	MD = 1,2
		実際に読み書きしたバイト数(負数ならError code)
	MD = 3〜5
		Error code

	モード MD の値に従って common 領域を制御する.
	この DOS コールによりプロセス間通信が簡単に実現出来る.

	MD = 0
		NAME で指定した common 領域が存在するか調べ、存在した場合は領
		域の大きさを返す.
	MD = 1
		NAME で指定した common 領域の、POS で指定した位置から LEN で指
		定したバイト数を、BUFFER で指定したバッファに読み込む.
	MD = 2
		NAME で指定した common 領域に、POS で指定した位置から LEN で指
		定したバイト数だけ、BUFFER で指定したバッファから書き込む.
		NAME で指定した common 領域が存在しない場合は作成する.
		LEN に 0 を指定すると切り詰める.
	MD = 3
		NAME で指定した common 領域の、POS で指定した位置から LEN で指
		定したバイト数を、ID_PSP でプロセスを指定してロックする.
		ID_PSP で指定されたプロセス以外のアクセスが禁止される.
	MD = 4
		NAME で指定した common 領域の、POS で指定した位置から LEN で指
		定したバイト数を、ID_PSP でプロセスを指定してロックを解除する.
		ID_PSP で指定されたプロセス以外のアクセスが許可される.
	MD = 5
		NAME で指定した common 領域を削除する.

MD = 0,5
	pea	(NAME)
	move	MD,-(sp)
	DOS	_COMMON
	addq.l	#6,sp

MD = 1,2
	move.l	LEN,-(sp)
	pea	(BUFFER)
	move.l	POS,-(sp)
	pea	(NAME)
	move	MD,-(sp)
	DOS	_COMMON
	lea	(18,sp),sp

MD = 3,4
	move.l	LEN,-(sp)
	move.l	ID_PSP,-(sp)
	move.l	POS,-(sp)
	pea	(NAME)
	move	MD,-(sp)
	DOS	_COMMON
	lea	(18,sp),sp

==============================================================================

$ff86	_RENAME		ファイル名の変更/移動

Arg	OLD.l		ファイル名のポインタ
	NEW.l		新ファイル名のポインタ

Ret	Error code

	OLD で指定したファイルを、NEW で指定したファイル名に変更する. システ
	ム属性・読み込み専用属性のファイルはリネーム出来ない.
	OLD と NEW のパスが異なる場合はファイルを移動するが、異なるドライブ間
	の移動や、ディレクトリやボリュームラベル、隠し属性・読み込み専用属性
	のファイルの移動は出来ない.
	Human68k version 3.02 では正常に動作しない(新旧両方のファイルが存在し、
	かつ双方の先頭 FAT 番号及びファイルサイズの上位ワードが等しい場合に、
	新ファイル名に変名される. また、移動先のディレクトリに空きがない場合に
	ファイルが消滅する. リネームが移動と見なされることがある).

	pea	(NEW)
	pea	(OLD)
	DOS	_RENAME
	addq.l	#8,sp

==============================================================================

$ff87	_FILEDATE	ファイルの日付/時間の読み出し/設定

Arg	FILENO.w	ファイルハンドル
	DATETIME.l	日付及び時間

Ret	DATETIME = 0
		ファイルの日時
	DATETIME = その他
		上位ワードが $ffff の時のみError code
		このコールでは負数でもError codeではない場合がある.

	FILENO で指定したファイルハンドルの日付と時間の収得/設定をする.
	DATETIME = 0 の場合は収得、それ以外の場合は設定を行う.

	%yyyy_yyym_mmmd_dddd_tttt_tfff_fffs_ssss
	bit 31〜25	年-1980(0〜127)
	bit 24〜21	月  (1〜12 月)
	bit 20〜16	日  (1〜31 日)
	bit 15〜11	時  (0〜23 時)
	bit 10〜 5	分  (0〜59 分)
	bit  4〜 0	秒/2(0〜29 秒)

	秒は 2 秒単位で表し、実際の秒を 2 で割った値を設定する.

	move.l	DATETIME,-(sp)
	move	FILENO,-(sp)
	DOS	_FILEDATE
	addq.l	#6,sp

==============================================================================

$ff88	_MALLOC2	指定の方法によるメモリの確保

Arg	MD.w		コントロールモード
	LEN.l		確保するバイト数
	PTR.l		管理するプロセスのプロセス管理ポインタ

Ret	確保したメモリブロックのポインタ
		d0.l = $81??????	指定された量は確保不可能
					($00?????? は確保可能なバイト数)
		d0.l = $8200000?	完全に確保不可能($? は不定)

	LEN で指定したバイト数のメモリを確保する. LEN が $01000000(16M)以上の
	場合は必ずエラーになり、確保可能な最大バイト数を調べる事が出来る. MD
	にはメモリを確保する方法を指定する.

	MD = 0	下位から検索
	MD = 1	必要最小ブロックから検索
	MD = 2	上位から検索

	Human68k version 3.00 以降では、MD の値が以下の値の時、親のプロセス管
	理ポインタの先頭アドレスを PTR で指定することで、常駐プログラムやデバ
	イスドライバからのメモリ確保が可能である.

	MD = $8000	下位から検索
	MD = $8001	必要最小ブロックから検索
	MD = $8002	上位から検索

[MD = 0,1,2]
	move.l	LEN,-(sp)
	move	MD,-(sp)
	DOS	_MALLOC2
	addq.l	#6,sp

[MD = $8000,$8001,$8002]
	pea	(PTR)
	move.l	LEN,-(sp)
	move	MD,-(sp)
	DOS	_MALLOC2
	lea	(10,sp),sp

==============================================================================

$ff8a	_MAKETMP	テンポラリファイルの作成

Arg	FILE.l		ファイル名のポインタ
	ATR.w		ファイル属性(%XLAD_VSHR)

Ret	作成したファイルのファイルハンドル(負数ならError code)

	FILE で指定したテンポラリファイルを ATR で指定した属性で作成する. ATR
	が 0 の場合はアーカイブ属性(%0010_0000)と見なされる.
	ファイル名には '?' を含めることができ、作成時に数字に置き換えられて存
	在しないファイル名になるまで加算して検索される. 数字を指定した場合はそ
	の数字から検索される.
	FILE で指定したファイル名はこの DOS コールにより書き換えられるので注意
	すること.

	move	ATR,-(sp)
	pea	(FILE)
	DOS	_MAKETMP
	addq.l	#6,sp

==============================================================================

$ff8b	_NEWFILE	ファイルの作成

Arg	FILE.l		ファイル名のポインタ
	ATR.w		ファイル属性(%XLAD_VSHR)

Ret	作成したファイルのファイルハンドル(負数ならError code)
		d0.l = -80	指定したファイルは既に存在している

	FILE で指定したファイルを ATR で指定した属性で作成する. ATR が 0 の
	場合はアーカイブ属性(%0010_0000)と見なされる.
	指定したファイルが既に存在している場合はエラー(-80)になる.

	move	ATR,-(sp)
	pea	(FILE)
	DOS	_NEWFILE
	addq.l	#6,sp

==============================================================================

$ff8c	_LOCK		ファイルのロック

Arg	MD.w		コントロールモード
	FILENO.w	ファイルハンドル
	OFFSET.l	ファイル先頭からのオフセット
	LEN.l		ロック/ロック解除するバイト数

Ret	Error code

	モード MD の値に従って、FILENO で指定したファイルのに対するアクセスの
	ロック/ロック解除を行なう.
	ファイルのロックとは、ファイルアクセスの排他制御で、他のプロセスからの
	ファイルアクセスを禁止することである.

	MD = 0
		OFFSET で指定した位置から LEN で指定したバイト数だけロックする.
	MD = 1
		OFFSET で指定した位置から LEN で指定したバイト数だけロックを解
		除する.

	move.l	LEN,-(sp)
	move.l	OFFSET,-(sp)
	move	FILENO,-(sp)
	move	MD,-(sp)
	DOS	_LOCK
	lea	(12,sp),sp

==============================================================================

$ff8f	_ASSIGN		仮想ドライブ/ディレクトリの割り当ての収得/設定/解除

Arg	MD.w		コントロールモード
	BUFFER1.l	仮想ドライブ・仮想ディレクトリのポインタ
	BUFFER2.l	実ドライブ・実ディレクトリのポインタ
	MODE.w		割り当てモード

Ret	MD = 0
		割り当てモード(負数ならError code)
			d0.l = $00	ドライブが存在しない
			d0.l = $40	割り当てが存在しない(実ドライブ)
			d0.l = $50	仮想ドライブの割り当て
			d0.l = $60	仮想ディレクトリの割り当て
	MD = 1,4
		Error code

	モード MD で指定した値に従って、仮想ドライブ・仮想ディレクトリの割り当
	ての収得/設定/解除を行なう.
	Human68k version 3.02 では正常に動作しない(補完モードで _FILES してい
	る為、違うディレクトリ名へ割り当ててしまう. また、".." 及び "."  エン
	トリのないサブディレクトリには割り当てられない).

	MD = 0
		BUFFER1 で指定したドライブの割り当てを収得し、BUFFER2 に書き込む.
	MD = 1
		割り当てモード MODE で指定した値に従って、BUFFER1 で指定した
		ドライブに BUFFER2で指定したディレクトリを割り当てる.

		MODE = $50(仮想ドライブ)
			BUFFER1 のドライブに BUFFER2 のディレクトリを割り当てる.
		MODE = $60(仮想ディレクトリ)
			BUFFER2 のディレクトリに BUFFER1 のドライブを割り当てる.
	MD = 4
		BUFFER1 で指定したドライブの割り当てを解除する.

	MD = 0

	pea	(BUFFER2)
	pea	(BUFFER1)
	move	MD,-(sp)
	DOS	_ASSIGN
	lea	(10,sp),sp

	MD = 1

	move	MODE,-(sp)
	pea	(BUFFER2)
	pea	(BUFFER1)
	move	MD,-(sp)
	DOS	_ASSIGN
	lea	(12,sp),sp

	MD = 4

	pea	(BUFFER1)
	move	MD,-(sp)
	DOS	_ASSIGN
	addq.l	#6,sp

==============================================================================

$ffaa	_FFLUSH_SET	FFLUSH モードの設定

Arg	MODE.w		モード
		MODE =	0	_FFLUSH を無効にする
			1	_FFLUSH を有効にする
			-1	現在のモードを収得する

Ret	変更前のモード

	DOS _FFLUSH の有効、無効を制御する. プログラム内で _FFLUSH を使用して
	頻繁にバッファをフラッシュしている時、FASTIO.X、FASTOPEN.X、FASTSEEK.X
	などの外部コマンドを常駐していても、高速化があまり有効にならない場合が
	ある. このような場合は、このコールで _FFLUSH を無効にしておくことによ
	り、高速化を有効にすることが出来る. ただし、バッファのフラッシュを実行
	しない事は危険なので、通常の使用では有効にしておくこと.

	このコールは Human68k version 3.00 以降で使用可能.

	move	#MODE,-(sp)
	DOS	_FFLUSH_SET
	addq.l	#2,sp

==============================================================================

$ffab	_OS_PATCH	OS 内部処理切り換え(システム専用コール)

Arg	PATCH_NO.w	モード及び機能番号(MD.b×256＋ID.b)
	PATCH_ADR.l	アドレス

Ret	MD = 0
		変更前の分岐命令表のアドレス
	MD = その他、または PATCH_ADR = 0
		Error code

	Human68k の内部処理を切り換える. パッチする機能番号を ID で、分岐命令
	表を PATCH_ADR で指定することにより、任意の処理に変更できる. ID の内容
	は以下の通り.

	ID = 1	FASTIO.X
	ID = 2	FASTSEEK.X
	ID = 3	FASTOPEN.X
	ID = 4	TwentyOne.x

	PATCH_ADR を 0 にするか、MD に 0 以外を指定すると、パッチ処理は行わず
	に最初のエントリ(常駐解除)を呼び出す. その際 MD.b を d1.w の上位 8 バ
	イトに、PATCH_ADR を d2.l に代入して呼び出すので、常駐解除以外の常駐部
	とのインターフェイスとして使用することも出来る.

	また、PATCH_ADR に -1 を指定すると、パッチは行なわずに 2 番目のエント
	リ(クローズ)及び 3 番目(初期化)を呼び出すので、常駐しているプログラム
	の初期化を行うことが出来る.

	分岐命令表は、各機能の処理アドレスに分岐する jmp (abs).l 命令を 18 個
	(ID = 3 の時)、または 20 個(ID = 1、2、4 の時)並べたもの.

	このコールは Human68k version 3.00 以降で使用可能だが、システム内部の
	詳細な知識が必要な為、システム専用とする.

	pea	(PATCH_ADR)
	move	#PATCH_NO,-(sp)
	DOS	_OS_PATCH
	addq.l	#6,sp

==============================================================================

$ffac	_GETFCB		Get FCB pointer

Arg	FILENO.w	ファイルハンドル

Ret	FCB のポインタ(負数ならError code)

	XC では _GET_FCB_ADR という名称だが、_GETFCB の方が一般的である.
	FILENO で指定したファイルの FCB のポインタを返す. OS 内部の FCB をその
	まま返すので、不用意に書き換えたりしないこと. また、このアドレスはスー
	パバイザ領域であるのでユーザモードではアクセス出来ない.

	このコールは Human68k version 3.00 以降で公開されたが、実際には
	version 3.00 未満でも使用することが出来る.

	FCB の大きさは 96 バイトで、内容は以下の通り.

offset	size
$00	1.b	この FCB に対応しているファイルハンドルの数
$01	1.b	デバイス情報(bit 7=0 の時は bit 4〜0 でドライブ名を保持する)
			bit 7	0:ブロックデバイス 1:キャラクタデバイス
			bit 6	更新フラグ
			bit 5	0:Local 1:Remote  ブロックデバイスの場合
			bit 5	0:COOKED 1:RAW	┐
			bit 3	CLOCK	デバイス│
			bit 2	NUL	〃	│キャラクタデバイスの場合
			bit 1	標準出力〃	│
			bit 0	標準入力〃	┘
$02	1.l	ブロックデバイス   : DPB のアドレス
		キャラクタデバイス : デバイスドライバへのポインタ
$06	1.l	ファイルポインタ
$0a	1.l	排他制御情報へのポインタ
$0e	1.b	アクセスモード
$0f	1.b	ディレクトリ上の位置(セクタ先頭からの個数:0〜31)
		(キャラクタデバイスでは未使用)
$10	1.b	アクセス中のクラスタ中のセクタ
$11	1.b	(未使用)
$12	1.w	アクセス中のクラスタ番号
$14	1.l	アクセス中のセクタ番号
$18	1.l	I/O バッファ先頭
$1c	1.l	対応するディレクトリエントリのセクタ番号
		(キャラクタデバイスでは未使用)
$20	1.l	最終アクセスポインタ(_SEEK しなければファイルポインタと同じ値)
$24	8.b	ファイル名１(余白は $20)
$2c	3.b	ファイル名拡張子(〃)
$2f	1.b	ファイル属性
$30	10.b	ファイル名２(〃)
$3a	1.w	最終更新時刻
$3c	1.w	最終更新年月日
$3e	1.w	先頭の FAT 番号
$40	1.l	ファイルサイズ
$44	7.l	FAT キャッシュ(上位word:先頭からのクラスタ数 下位word:FAT 番号)

	move	FILENO,-(sp)
	DOS	_GETFCB
	addq.l	#2,sp

==============================================================================

$ffad	_S_MALLOC	メインのメモリ管理下からのメモリブロックの確保する

Arg	MD.w		コントロールモード
	LEN.l		確保するバイト数
	PTR.l		管理するプロセスのプロセス管理ポインタ

Ret	確保したメモリブロックのポインタ
		d0.l = $81??????	指定された量は確保不可能
					($00?????? は確保可能なバイト数)
		d0.l = $8200000?	完全に確保不可能($? は不定)

	メインのメモリ管理下から LEN で指定したバイト数のメモリを確保する. LEN
	が $01000000(16M)以上の場合は必ずエラーになり、確保可能な最大バイト数
	を調べる事が出来る. MD にはメモリを確保する方法を指定する.

	MD = 0	下位から検索
	MD = 1	必要最小ブロックから検索
	MD = 2	上位から検索

	Human68k version 3.00 以降では、MD の値が以下の値の時、親のプロセス管
	理ポインタの先頭アドレスを PTR で指定することで、常駐プログラムやデバ
	イスドライバからのメモリ確保が可能である.

	この DOS コールは、通常のアプリケーションプログラムで使用してはいけな
	い. 常駐するプログラムのみ使用出来る.

[MD = 0,1,2]
	move.l	LEN,-(sp)
	move	MD,-(sp)
	DOS	_S_MALLOC
	addq.l	#6,sp

[MD = $8000,$8001,$8002]
	pea	(PTR)
	move.l	LEN,-(sp)
	move	MD,-(sp)
	DOS	_S_MALLOC
	lea	(10,sp),sp

==============================================================================

$ffae	_S_MFREE	メインのメモリ管理下のメモリブロックの解放

Arg	MEMPTR.l	メモリ管理ポインタ

Ret	Error code

	MEMPTR で指定した、メインのメモリ管理下のメモリブロックを解放する.
	MEMPTR が S_PROCESS で指定したサブのメモリ管理ポインタで、かつそのスレ
	ッド ID がカレント ID なら、KILL_PR でプロセスを削除する. その場合サブ
	のメモリ管理下で常駐終了しているプロセスは、メインのメモリ管理下に入る.
	MEMPTR に 0 を指定した場合、自分が確保したメモリを全て解放する.
	Human68k version 3.02 では正常に動作しない(常駐終了していたプロセスの
	メモリブロックが破壊される).

	pea	(MEMPTR)
	DOS	_S_MFREE
	addq.l	#4,sp

==============================================================================

$ffaf	_S_PROCESS	サブのメモリ管理の設定

Arg	ID.w		スレッド ID
	START.l		サブのメモリ管理ポインタ
	LENGTH.l	メモリブロック全体のバイト数
	I_LEN.l		先頭ブロックのバイト数

Ret	先頭のメモリブロック(サイズはI_LEN)のポインタ
		d0.l = $ffff00??	ID エラー($?? は ID の最大値)
		d0.l = -14		LENGTH が I_LEN＋16 より小さい

	START で指定した先頭アドレスと LENGTH で指定したバイト数で、サブのメモ
	リ管理を設定する. サブのメモリ管理を新しく設定する際には、必ず先頭から
	1 ブロックだけ自動的に確保されるので、そのバイト数 I_LEN を指定する.
	以後指定した ID を持つスレッドからのメモリ確保要求は、このサブのメモリ
	管理下に限定される.

	move.l	I_LEN,-(sp)
	move.l	LENGTH,-(sp)
	pea	(START)
	move	ID,-(sp)
	DOS	_S_PROCESS
	lea	(14,sp),sp

==============================================================================

$fff0	_EXITVC		(プログラム終了時の実行アドレス)

	これは DOS コールではなく、INTVCS でこのベクタに処理アドレスを設定して
	おくことにより、プログラム終了時にそのアドレスを呼び出すことが出来る.

	通常は親プロセスが子プロセスを起動する為に実行した DOS _EXEC の直後の
	アドレスが設定され、子プロセスが DOS _EXIT 等で終了した時に DOS コール
	から戻るアドレスとして参照される. 呼び出されるのは DOS コールの処理が
	全て終わった後であるから、DOS _EXIT 等を実行したプロセスは既に削除され、
	元の親プロセスに制御が移っている状態である.

	このベクタはプロセスの起動ごとに書き換えられ、PSP に複写される. 実際に
	参照されるのは PSP 内の値であるので、直接 DOS コールのベクタテーブルを
	書き換えても反映されない.

==============================================================================

$fff1	_CTRLVC		(CTRL+C によるアボート時の実行アドレス)

	これは DOS コールではなく、INTVCS でこのベクタに処理アドレスを設定して
	おくことにより、ブレークでアボートした時にそのアドレスを呼び出すことが
	出来る.

	初期状態では単に DOS _EXIT を実行するだけのルーチンが設定されている.
	CTRL+C によるアボートは DOS コール実行中にのみ発生するが、設定したアド
	レスは DOS コールを強制終了してから呼び出される(呼び出された時は既に
	DOS コール実行中ではなく、ネストしていてもその時点で完全に終了する).

	このベクタは自動的には変更されず、変更した場合は子プロセス全てがその値
	を継承する.

==============================================================================

$fff2	_ERRJVC		(エラーによるアボート時の実行アドレス)

	これは DOS コールではなく、INTVCS でこのベクタに処理アドレスを設定して
	おくことにより、エラーでアボートした時にそのアドレスを呼び出すことが出
	来る.

	初期状態では単に DOS _EXIT を実行するだけのルーチンが設定されている.

	このベクタは自動的には変更されず、変更した場合は子プロセス全てがその値
	を継承する.

==============================================================================

$fff3	_DISKRED	ブロックデバイスから直接読み込む

Arg	BUFFER.l	読み込むバッファのポインタ
	DRIVE.w		ドライブ番号(0:カレント 1:A 2:B … 26:Z)
	SECT.{w/l}	読み込む最初のセクタ番号
	SECTLEN.{w/l}	読み込むセクタ数

	DRIVE で指定したブロックデバイスの、SECT で指定したセクタから、SECTLEN
	で指定したセクタ数だけ BUFFER で指定したバッファに読み込む.

	この DOS コールは、バッファのアドレス BUFFER をスタックに積む時
	[1] そのまま積む
	[2] BUFFER＋$80000000 を積む
	の二通りのコール方法があり、それぞれArgの大きさが違う.

	[1] のそのまま積む方法の場合、SECT 及び SECTLEN はワードサイズで指定す
	る. この方法では、1 セクタ当りのバイト数が 1024 以外のドライブ(512 バ
	イトのドライブやリモートドライブなど)から読み込むことは出来ず、エラー
	になる. 読み込みはセクタ単位で行うので、バッファはセクタ数×1024 バイ
	ト用意すること.

	[2] の BUFFER＋$80000000 を積む方法は大容量ドライブ・特殊ドライブのた
	めに用意されたもので、SECT 及び SECTLEN をロングワードサイズで指定する.
	こちらはセクタ当りのバイト数が 1024 でないドライブやリモートドライブか
	らも読み込めるが、1 セクタ当りのバイト数が 1024 とは限らないのでバッフ
	ァ容量には注意すること.

	いずれの方法でも、SECT は 0 以上、SECTLEN は 1 以上で、上限はメディア
	によって異なる.

[1]
	move	SECTLEN,-(sp)
	move	SECT,-(sp)
	move	DRIVE,-(sp)
	pea	(BUFFER)
	DOS	_DISKRED
	lea	(10,sp),sp

[2]
	move.l	SECTLEN,-(sp)
	move.l	SECT,-(sp)
	move	DRIVE,-(sp)
	pea	(BUFFER+$80000000)
	DOS	_DISKRED
	lea	(14,sp),sp

==============================================================================

$fff4	_DISKWRT	ブロックデバイスへ直接書き込む

Arg	BUFFER.l	書き込むバッファのポインタ
	DRIVE.w		ドライブ番号(0:カレント 1:A 2:B … 26:Z)
	SECT.{w/l}	書き込む最初のセクタ番号
	SECTLEN.{w/l}	書き込むセクタ数

	BUFFER で指定したバッファから、DRIVE で指定したブロックデバイスの SECT
	で指定したセクタに、SECTLEN で指定したセクタ数だけ書き込む.

	この DOS コールは、バッファのアドレス BUFFER をスタックに積む時
	[1] そのまま積む
	[2] BUFFER＋$80000000 を積む
	の二通りのコール方法があり、それぞれArgの大きさが違う.

	[1] のそのまま積む方法の場合、SECT 及び SECTLEN はワードサイズで指定す
	る. この方法では、1 セクタ当りのバイト数が 1024 以外のドライブ(512 バ
	イトのドライブやリモートドライブなど)に書き込むことは出来ず、エラーに
	なる. 書き込みはセクタ単位で行うので、データはセクタ数×1024 バイトだ
	け用意しておくこと.

	[2] の BUFFER＋$80000000 を積む方法は大容量ドライブ・特殊ドライブのた
	めに用意されたもので、SECT 及び SECTLEN をロングワードサイズで指定する.
	こちらはセクタ当りのバイト数が 1024 でないドライブやリモートドライブに
	も書き込めるが、1 セクタ当りのバイト数が 1024 とは限らないので用意する
	データ量には注意すること.

	いずれの方法でも、SECT は 0 以上、SECTLEN は 1 以上で、上限はメディア
	によって異なる.

[1]
	move	SECTLEN,-(sp)
	move	SECT,-(sp)
	move	DRIVE,-(sp)
	pea	(BUFFER)
	DOS	_DISKWRT
	lea	(10,sp),sp

[2]
	move.l	SECTLEN,-(sp)
	move.l	SECT,-(sp)
	move	DRIVE,-(sp)
	pea	(BUFFER+$80000000)
	DOS	_DISKWRT
	lea	(14,sp),sp

==============================================================================

$fff5	_INDOSFLG	OS ワーク内へのポインタを得る

Ret	INDOS_FLG のポインタ

	OS のワーク INDOS_FLG へのポインタを返す. スーパーバイザ領域のため、ユ
	ーザーモードではアクセス出来ない. また、INDOS_FLG 以後には OS にとって
	重要なワークが存在するので、絶対に書き換えてはいけない.
	INDOS_FLG の内容は以下の通り.

offset	size
0	1.w	indos_f		OS 実行中のレベル
2	1.b	doscmd		OS 実行中ファンクション番号
3	1.b	fat_flg		FAT 検索モード(0:標準 2:常に先頭から)
4	1.w	retry_count	I/O リトライ回数(標準で 3 回)
6	1.w	retry_time	リトライ待ち時間(標準で 100 = 1 秒)
8	1.w	verifyf		ベリファイモード(0:Off 1:On)
10	1.b	breakf		ブレークモード(0:Off 1:On)
11	1.b	ctrlpf		CTRL+P モード(0:Off 1:On)
12	1.b	(未公開)	スレッド切り換え要求フラグ
13	1.b	wkcurdrv	カレントドライブ(A=0,B=1,…,Z=25)

==============================================================================

$fff6	_SUPER_JSR	スーパーバイザモードでのプログラムのサブルーチンコール

Arg	JOBADR.l	処理アドレス
	d0-d7/a0-a6	処理に依存

Ret	d0-d7/a0-a6	処理に依存

	JOBADR で指定したプログラムにスーパーバイザモードで jsr する.
	DOS コールを実行する直前の d0-d7/a0-a6 が渡され、処理が戻るときにはそ
	の時の d0-d7/a0-a6 が返るが、SR は変化しない.
	SSP/USP はどのように渡されるか不定なので、スタックを用いてArgを渡すこ
	とは出来ない.
	この DOS コールでは、jsr することにより発生する暴走やバスエラーに対す
	る処理を全く行わない.

	pea	(JOBADR)
	DOS	_SUPER_JSR
	addq.l	#4,sp

==============================================================================

$fff7	_BUS_ERR	バスエラーが発生するかの検査

Arg	SIZE.w		アクセスサイズ(1:バイト 2:ワード 4:ロングワード)
	P1.l		読み込みポインタ
	P2.l		書き込みポインタ

Ret	d0.l =  0	読み書き可能
	d0.l =  1	P2 に書き込んだ時にバスエラーが発生
	d0.l =  2	P1 から読み込んだ時にバスエラーが発生
	d0.l = -1	エラー(Argが異常)

	SIZE で指定されたサイズで P1 で指定したアドレスから読み込み、そのデータ
	を P2 で指定したアドレスに書き込んでバスエラーが発生するかどうか調べる.
	SIZE の値が異常な場合や SIZE = 2,4 で P1,P2 に奇数アドレスを指定した場
	合はRetが -1 になる.

	move	SIZE,-(sp)
	pea	(P2)
	pea	(P1)
	DOS	_BUS_ERR
	lea	(10,sp),sp

==============================================================================

$fff8	_OPEN_PR	バックグラウンドタスクの登録

Arg	NAME.l		スレッド名のポインタ
	COUNT.w		実行間隔を決める値
	INIT_USP.l	USP の初期値
	INIT_SSP.l	SSP の初期値
	INIT_SR.w	SR の初期値
	INIT_PC.l	プログラムの実行アドレス
	BUFFER.l	タスク間通信バッファのポインタ
	SLEEP_TIME.l	待ち時間(ms)

Ret	登録したタスクのスレッド ID(負数ならError code)
		d0.l = -27	既に同名タスクが存在する.
		d0.l = -29	これ以上タスクを登録出来ない.

	NAME で指定した 15 文字以内のスレッド名のバックグラウンドタスクを登録
	する. 登録したスレッドはスリープ状態になる.
	COUNT には、タスクを 1 回実行するのにタイマ割り込みを何回カウントする
	かの回数を 2〜255 で指定する. 0〜1 の場合は 2 として扱う.
	INIT_USP,INIT_SSP,INIT_PC は登録したタスクを実行する時のそれぞれのレジ
	スタの初期値. システム用のスタックは 6KB 必要.
	INIT_SR は $0000 若しくは $2000 を指定し、ユーザーモードかスーパーバイ
	ザーモードのどちらかを撰択する.
	その他のレジスタの初期値は全て 0.
	BUFFER はタスク間通信のバッファのアドレスを指定する.
	SLEEP_TIME は待ち時間をミリ秒単位で指定する. 0 を指定すると永久にスリ
	ープする.
	DOS _OPEN_PR によりバックグラウンドタスクとして登録されたスレッドは、
	DOS _KEEPPR で常駐終了する.
	スレッドをメモリ上から消去する場合は DOS _KILL_PR を使用する.
	BUFFER の内容は以下の通り.

offset	size
0	1.l	LENGTH	データ用バッファのバイト数
4	1.l	DATABUF	データ用バッファの先頭アドレス
8	1.w	COMMNAD	コマンド番号
10	1.w	ID	相手の ID(-1 で通信許可)

	move.l	SLEEP_TIME,-(sp)
	pea	(BUFFER)
	pea	(INIT_PC)
	move	SR,-(sp)
	pea	(INIT_SSP)
	pea	(INIT_USP)
	move	COUNT,-(sp)
	pea	(NAME)
	DOS	_OPEN_PR
	lea	(28,sp),sp

==============================================================================

$fff9	_KILL_PR	自分自身のプロセスの削除

Ret	Error code

	自分自身のプロセスを削除する. 常駐終了していたプロセスの場合、同じ
	プロセス ID を持つスレッド全てを削除し、確保されていたメモリを解放する.
	この DOS コールを実行する前には、自分でオープンしたファイルは全て
	クローズし、書き換えたベクタなども元に戻す. システムはスレッドの削除と
	メモリの解放しか行わない.
	プロセス内でいくつかの登録したスレッドを実行した後で DOS _EXIT や
	DOS _EXIT2 で終了する場合、先にオープンしたスレッドを KILL_PR で削除す
	る. メインのスレッドであるプロセスを削除した場合、その後の動作は保証さ
	れない. 自分以外のプロセスを削除する場合は、DOS _SEND_PR で KILL コマ
	ンドを送る.

==============================================================================

$fffa	_GET_PR		Get thread management information

Arg	ID.w		Thread ID
	BUFFER.l	Pointer to buffer to write management information

Ret	Thread ID or Error code

Copies the management information of the thread with the specified ID to the 116-byte buffer specified by BUFFER.
If -1 is specified for ID and thread name is specified for BUFFER + 96, the ID of the thread is returned.
Also, if you specify -2 for ID, your own ID will be returned.

offset	size
0	1.l	NEXT_PTR	Management information storage area of the next thread
4	1.b	WAIT_FLG	0:Normal -1:Wait
5	1.b	COUNT		subtraction counter
6	1.b	MAX_CNT		Initial value of COUNT
7	1.b	DOSCMD		DOS call number
8	1.l	PSP_ID		Process ID
12	1.l	USP
16	8.l	D0,D1,D2,D3,D4,D5,D6,D7
48	7.l	A0,A1,A2,A3,A4,A5,A6
76	1.w	SR
78	1.l	PC
82	1.l	SSP
86	1.w	INDOSF		system reservation (OS running level)
88	1.l	INDOSP		system reservation (OS running stack)
92	1.l	BUFF		Inter-task communication buffer
96	16.b			Thread name
112	1.l	WAIT_TIME	Wait time remaining (ms)

	The size of the actual management information is 124 bytes, but only 116 bytes are available from the beginning.

	pea	(BUFFER)
	move	ID,-(sp)
	DOS	_GET_PR
	addq.l	#6,sp

==============================================================================

$fffb	_SUSPEND_PR	スレッドを強制的にスリープ状態にする

Arg	ID.w		スレッド ID

Ret	Error code
		d0.l = 0		正常終了
		d0.l < 0		Error code
		d0.l = -1		スレッド自身のエラー
		d0.l = $ffff00??	ID が異常($?? は ID の最大値)

	ID で指定したスレッドを強制的にスリープ状態にする. スリープ状態になっ
	たスレッドは、DOS _SEND_PR で起こされるまでスリープする.

	move	ID,-(sp)
	DOS	_SUSPEND_PR
	addq.l	#2,sp

==============================================================================

$fffc	_SLEEP_PR	スリープ状態に入る

Arg	TIME.l		待ち時間(ミリ秒単位)

Ret	スレッドの状態
		d0.l = -1	待ち時間が経過し、自分で起きた.
		d0.l = -2	待ち時間が経過した後 DOS _SEND_PR で起こされた.
		d0.l = その他	残り時間(DOS _SEND_PR で起こされた).

	TIME で待ち時間を指定してスリープ状態に入る.
	TIME = 0 の時、永久にスリープする.
	スリープ状態に入ったスレッドは DOS _SEND_PR を用いて強制的に起こすこと
	が出来る. DOS _SEND_PR のコマンドが $fffb の場合、タスク間通信バッファ
	は変化しない. それ以外のコマンドの場合は、以下の通りに設定される.

offset	size
0	1.l	LENGTH	データバッファに書き込まれたバイト数
4	1.l	DATABUF	データバッファのポインタ
8	1.w	COMMAND	コマンド番号
10	1.w	ID	起こしたスレッドの ID

	スリープする前にデータバッファの内容を処理してからデータバッファのアド
	レスとバイト数を設定し、ID に -1 を設定する. これにより、他のスレッド
	からの通信を許可することになる.
	スリープしていない時でも SEND_PR によりデータが送られた場合は、スリー
	プするとすぐに起こされ設定した待ち時間を返す.

	Retの内容は以下の通り.

	d0.l = -1
		待ち時間が経過して自分で起きた.
		タスク間通信バッファの内容は変化しない.
	d0.l = -2
		待ち時間が経過したが、DOS _SUSPEND_PR で止められた後 DOS _SEND_PR
		で起こされた.
	d0.l = その他
		待ち時間は経過していないが、DOS _SEND_PR で起こされた.
		Retの単位はミリ秒.

	move.l	TIME,-(sp)
	DOS	_SLEEP_PR
	addq.l	#4,sp

==============================================================================

$fffd	_SEND_PR	スレッドにコマンド/データを送り、スリープしていたら起こす

Arg	MY_ID.w		自分のスレッド ID
	YOUR_ID.w	相手のスレッド ID
	CMDNO.w		送出するコマンド
	BUFFER.l	送出するデータのポインタ
	LEN.l		送出するデータのバイト数

Ret	d0.l = 0		正常終了
	d0.l < 0		Error code
	d0.l = -28		書き込みエラー
	d0.l = $ffff00??	指定 ID が不正($?? は ID の最大値)
	d0.l = $8000????	データのバイト数が不正($???? は容量の最大値)

	指定した ID のスレッドにコマンドやデータを送り、スリープしていたら起こす.
	MY_ID に自分の ID、YOUR_ID に通信先のスレッドの ID を指定する.
	CMDNO は通信の内容を表すワード値で、システムで定められているコマンド以
	外はそれぞれのスレッド間で定義する. システムで予約しているコマンドは
	$ff?? で、以下に示す値が定義されている.

	$fff9	スレッドを消去するように要求する.
	$fffb	強制スリープ状態から起こすだけで、タスク間通信バッファは変化しない.
	$fffc	スリープするように要求する.
		起こされた時にこのコマンドが送られていたら、すぐにタスク間通信
		バッファの ID を -1 にしてスリープするべきである.
		スリープしないでタスク間通信バッファを監視している場合に有効.
	$ffff	処理が終ったかどうか調べる. -28 が返れば、まだ処理中である.

	コマンドやデータの転送には、タスク間通信バッファが使用される.
	バッファの内容は以下の通り.

offset	size
0	1.l	LENGTH	データバッファのバイト数
4	1.l	DATABUF	データバッファのポインタ
8	1.w	COMMAND	コマンド番号
10	1.w	ID	ID(-1 で通信許可)

	YOUR_ID で指定したスレッドの、タスク間通信バッファの ID が -1 に設定さ
	れていたら、そのバッファは書き込み可能なので、MY_ID と CMDNO をそれぞ
	れ ID と COMMAND に設定し、BUFFER からのデータを LEN で指定したバイト
	数だけ DATABUF に書き込み、LENGTH を LEN にする. また、指定したスレッ
	ドがスリープしていたら起こす.
	特に $fffb のコマンドは特殊処理され、指定したスレッドを起こすだけでタ
	スク間通信バッファの ID が -1 でなくてもよく、その他のバッファも変化し
	ない.
	LEN が LENGTH より大きい時や書き込み不可の時はエラーになる.

	move.l	LEN,-(sp)
	pea	(BUFFER)
	move	CMDNO,-(sp)
	move	YOUR_ID,-(sp)
	move	MY_ID,-(sp)
	DOS	_SEND_PR
	lea	(14,sp),sp

==============================================================================

$fffe	_TIME_PR	タイマのカウンタ値を得る

Ret	現在のタイマのカウンタ値

	現在のタイマのカウンタ値(ミリ秒単位)を調べる.
	バックグラウンドで複数のスレッドが平行して動作している場合、プログラム
	で一定の時間を計るためには、どのスレッドが動作していても一定して変化す
	るカウンタが必要になる.
	ロングワードでの最大値を越えると 0 に戻るので、そのまま前回に返された
	値との減算で経過時間が分かる.

==============================================================================

$ffff	_CHANGE_PR	実行権を放棄

	バックグラウンドタスクの自分の実行権を放棄する. 次のタスクに切り換わる.

==============================================================================
```