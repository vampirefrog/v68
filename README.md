PSP = Program Segment Prefix, similar to DOS PSP

USP = User Stack Pointer
SSP = Supervisor Stack Pointer
BSS = space for global vars, initialized to zero at load

https://nfggames.com/X68000/index.php/Mirrors/Groundzero%20Organization/x68tools/develop/docs/puni/

puni7_2/oswork.man:320, メモリ管理ポインタ:
```

· Memory management pointer

offset   size
$00 (0)   1.l   Previous memory management pointer (leading with 0)
$04 (4)   1.l   Parent process memory management pointer (0 with no parent)
$08 (8)   1.l   Last address of this memory block + 1
$0c (12)  1.l   Next memory management pointer (end with 0)

It is a structure of 16 bytes created at the head of each memory block. The first memory block exists in the supervisor area inside Human 68k.

The most significant byte of the 4th byte (the process that secured memory) represents the attribute of that memory block.

$ff Resident process (KEEP)
$fe unknown (MEMDRV)
$fd Sub memory block

· Process management pointer

A 256-byte   structure (including a memory management pointer) for holding process information, which is created at the beginning of the memory block for each process.

offset   size
$00(0)    4.l   (memory management pointer)
$10(16)   1.l   Environment's address (not secured if -1)
$14(20)   1.l   Return address at the end
$18(24)   1.l   Return address when interrupted by CTRL + C
$1c(28)   1.l   Return address when interrupt due to error
$20(32)   1.l   command line address
$24(36)   12.b  File handler usage of process
                (In use = 1 in the order of bits 0 to 7 of $24 to $2f)
$30(48)   1.l   Start address of BSS
$34(52)   1.l   Start address of heap (same as BSS)
$38(56)   1.l   Initial stack address (end of heap + 1)
$3c(60)   1.l   USP of the parent process
$40(64)   1.l   parent process SSP
$44(68)   1.w   parent process SR
$46(70)   1.w   SR at abort
$48(72)   1.l   SSP at abort
$4c(76)   1.l   trap # 10 vector
$50(80)   1.l   vector of trap # 11
$54(84)   1.l   trap #12 vector
$58(88)   1.l   trap #13 vector
$5c(92)   1.l   trap #14 vector
$60(96)   1.l   shell activation flag (0: normal startup - 1: started as shell)
$64(100)  1.b   Module number
$65(101)  3.b   (unused)
$68(104)  1.l   Process management pointer of loaded child process
$6a(108)  5.l   (unused)
$80(128)  2. b  Drive name of executable file
$82(130)  66.b  Path name of executable file
$c4(196)  24.b  File name of executable file
$dc(220)  9.l   (unused)

$ff4b	_EXEC		Loading / executing programs

Arg	MD.w		Module number and mode (MODULE.b × 256 + MODE.w)
	FILE.l		Pointer to file name
	CMDLINE.l	Command line pointer
	LOADADR.l	Load address
	EXECADR.l	Execution address
	FILE2.l		Filename in overlay X file
	ENVPTR.l	Environment pointer
	LIMIT.l		Limit address
	TYPE.b		File type (upper 8 bits of FILE)

Return values
	MODE = 0,4
		Process end code (error code if negative number)
		When the process ends, d1-d7 / a1-a6 is undefined.
	MODE = 1
		d0.l	Execution address of the program (error code if negative number)
		a0.l	Memory management pointer
		a1.l	End of program
		a2.l	Command line
		a3.l	Environment pointer
		a4.l	Execution address
	MODE = 2
		Error code
	MODE = 3
		Program length (error code if negative number)
	MODE = 5
		Module number * 256 (error code if negative number)

	Load / execute the file specified by FILE according to the value of mode MODE.

	MODE = 0
		Specify the command line with CMDLINE, the environment with ENVPTR, load and execute the file specified by FILE.
	MODE = 1
		CMDLINE でコマンドラインを、ENVPTR で環境を指定して、FILE で指定したファイルをロードする. 正常終了した場合は必ず MODE = 4 を実行すること.
	MODE = 2
		ENVPTR で指定した環境から path を検索して、FILE で指定したコマンド行をフルパスのファイル名とコマンドラインに分け、FILE とCMDLINE の各ポインタに設定する.
		ファイル名バッファ FILE は 90 バイト以上、コマンドラインバッファ CMDLINE は 256 バイト以上必要.
	MODE = 3
		LOADADR でロードアドレスを、LIMIT でリミットアドレスを指定して、FILE で指定したファイルをロードする.
	MODE = 4
		EXECADR で指定したアドレスからプログラムを実行する.
		MD = 1 でロードした後、実行する時に使用する.
	MODE = 5
		FILE で指定したオーバーレイ X ファイル中の、FILE2 で指定したファイルのモジュール番号を調べる.
		バインドリスト先頭からのオフセットが $fffff00 より大きいファイルは、オフセット値をビット反転した数をモジュール番号として返す.
		ただし、このようなオフセット値が負数のファイルはロード出来ない.
		Human68k version 3.02 では正常に動作しない(FILE2 で指定したファイル名の拡張子 1 バイト目を X に変更したファイル名で検索される).

	WHen MODE = 0 to 2, when 0 is specified as ENVPTR, use your own environment.
	MODE = 0,1,3 の時、MODULE でオーバーレイ X ファイル中の各ファイルのモジュール番号(0〜255)を指定することが可能.
	また、実行ファイル名の拡張子が .X .Z .R のいずれでもない場合は、FILE の上位 8bitでファイルタイプ TYPE を指定し、それによってファイルの形式を指定する.

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

```


```
・Structure of .X file header

offset	size
$00	2.b	Identifier 'HU'(0x48 0x55)
$02	1.b	Reserved (0)
$03	1.b	Load mode (0:Normal 1:Minimum block 2:High address)
$04	1.l	Base address
$08	1.l	Entry point address
$0c	1.l	Text section size
$10	1.l	Data section size
$14	1.l	Block storage section size (including .comm, .stack)
$18	1.l	Relocation table size
$1c	1.l	Symbol table size
$20	1.l	SCD line number table size
$24	1.l	SCD symbol table size
$28	1.l	SCD string table size
$2c	4.l	Reserved (0)
$3c	1.l	Position of the bound module list from the top of the file

Size: 64 ($40) bytes

  実行ファイルは、ベースアドレスにロードして再配置処理が行われた状態で格納されている.
  よって、実際のロードアドレスがベースアドレスと等しければ再配置は行わなくて良い.
  また両者が等しくない場合は、その差を足す.
  実行開始アドレスもベースアドレスが加算された値が格納されているので、こちらも差を足す.

・Relocation

  再配置は以下の手続きを再配置テーブルのサイズ分繰り返す事によって行われる.

1) プログラム先頭アドレスを A、ベースアドレスを B とする.
2) A から B を引き、これを C とする.
3) 再配置テーブルから 1 ワード収得し、D とする.
4) D が 1 の場合、再配置テーブルから 1 ロングワード収得し、新しい D とする.
5) D の最下位ビットが 0(偶数)の場合は、A に D を加算し、A のアドレスからの
   1 ロングワードに C を足す.
6) D の最下位ビットが 1(奇数)の場合は、A に D-1 を加算し、A のアドレスからの
   1 ワードに C を足す(ワードサイズの再配置は通常使われる事はない).

・Sections

1. Header($40 bytes)
2. Text section
3. Data section
3-1. Section information ($40 byte)
    SX モードで作成された実行ファイルには、データセクションの先頭に仮想オブジェクト *SYSTEM* がリンクされ、セクション情報が出力される.
3-2.".data"
3-3.".rdata" の実体
3-4.".rldata" の実体
3-5.相対オフセットテーブル(n*4 バイト)
    SX モード作成された実行ファイルにおいて、".rdata" 及び ".rldata" 内で相対
    セクション内へのポインタを使用した場合に出力される.
4.Block storage section
  実体は出力されない.
4-1.".bss"
4-2.".common"
4-3.".stack"
5.Relocation table
6.Symbol table
7.SCD table
8.If bound, repeat 1-7
9.Bind list

  相対セクションは一種のオフセット定義であり、".rdata" と ".rldata" の実体を除きその領域は実行ファイルに出力されないが、以下の順で定義される.

1.".rdata"
2.".rbss"
3.".rcommon"
4.".rstack"
5.".rldata"
6.".rlbss"
7.".rlcommon"
8.".rlstack"

・Bound list

offset	size
$00	8.b	ファイル名($e5 で始まるファイル名は $05 とする. 残りはスペース
		で埋める.)
$08	3.b	拡張子(残りはスペースで埋める)
$0b	1.b	属性(%XLAD_VSHR. ただし ASHR 以外を指定しても当然エラーになる)
$0c	10.b	8 バイト以上のファイル名の残り(残りは $00 で埋める)
$16	1.w	Last modified time
		%mmmS_SSSS_HHHH_HMMM(H:時 Mm:分 S:秒)
$18	1.w	Last modified date
		%mmmD_DDDD_YYYY_YYYM(Y:年 Mm:月 D:日)
$1a	1.b	特殊属性の変更禁止フラグ┐BIND.X で使用
$1b	1.b	特殊属性(%00A0_0SHR)	┘
$1c	1.l	バインドファイル先頭からのオフセット

Size: 32($20) bytes

  ディレクトリエントリとほぼ同じ構造である.
  年月日及び時刻の上下バイトがディレクトリエントリ同様逆転しているので注意すること.
  ファイルの残りが 0 バイトでリストが終わり、32 バイト以上の時はまだ続きがあり、それ以外(1〜31 バイト)ならば異常な構造となる.
```


```

$ff47	_CURDIR		Get the current directory

Arg DRIVE.w		Drive number (0: Current, 1: A, 2: B ... 26: Z)
	BUFFER.l	Buffer pointer

Ret Error Code

	Writes the current directory of the drive specified by DRIVE into the buffer of 65 bytes specified by BUFFER.
	Drive name · Do not add '\' to root directory and '\' to terminate.

	pea	(BUFFER)
	move	DRIVE,-(sp)
	DOS	_CURDIR
	addq.l	#6,sp
```
