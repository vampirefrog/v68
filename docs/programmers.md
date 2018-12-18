```
	- PROGRAMMER'S MANUAL -

==============================================================================

・State at program startup

Registers

a0	Address of memory management pointer
a1	End address of program + 1
a2	Command-line address
a3	Environment address
a4	Program start address
sr	User mode
usp	Parent stack
ssp	System stack
Others	undefined

Memory
	The maximum memory that can be allocated is allocated.

Command-line structure
	LASCII format, length of character string is the first byte, and 0 is added at the end of the string.

Environment
	If you change the contents of the environment with DOS _SETENV in the parent environment itself, the parent's environment will also be changed.
	When executing a child process under different environments, separate memory is allocated and a new environment is prepared.

Structure of the environment
	The size of the environment variable area is indicated in the first 4 bytes.
	Each environment variable is delimited by one byte of 0, and the end of the environment variable is given another zero byte.

==============================================================================

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

The executable file is loaded in the base address and stored in a state where the relocation processing has been performed.
Therefore, if the actual load address is equal to the base address, rearrangement does not need to be performed.
If they are not equal, add the difference.
Since the value obtained by adding the base address to the execution start address is also stored, this also adds the difference.

· Relocation processing

   The relocation is performed by repeating the following procedure for the size of the relocation table.

1) Set the program start address to A and the base address to B.
2) Subtract B from A and let it be C.
3) Acquire one word from the relocation table and let it be D.
4) When D is 1, 1 longword is acquired from the relocation table and it is taken as new D.
5) When the least significant bit of D is 0 (even number), add D to A and add C to one longword from the address of A.
6) When the least significant bit of D is 1 (odd number), add D-1 to A and add C to 1 word from the address of A (word size relocation is never used normally) .

・Order of sections

1. Header ($40 bytes)
2. Text section
3. Data section
3-1. Section information ($ 40 bytes)
    The virtual file * SYSTEM * is linked to the beginning of the data section
    in the executable file created in SX mode, and section information is output.
3-2.".data"
3-3.".rdata" の実体
3-4.".rldata" の実体
3-5.Relative offset table (n * 4 bytes)
    SX mode This is output when a pointer to a relative section is used in ".rdata" and ".rldata" in the created executable file.
4. Block Storage Section
  Entities are not output.
4-1. ".bss"
4-2. ".common"
4-3. ".stack"
5. Relocation table
6. Symbol table
7. SCD table
8. If bound, repeat 1-7.
9. Bound list

  A relative section is a sort of offset definition, and except for the entities ".rdata" and ".rldata",
the area is not output to the executable file, but it is defined in the following order.

1.".rdata"
2.".rbss"
3.".rcommon"
4.".rstack"
5.".rldata"
6.".rlbss"
7.".rlcommon"
8.".rlstack"

・Bound list structure

offset	 size
$00	8.b	 File name (The file name beginning with $ e5 is $ 05, the rest is filled with space.)
$08	3.b	 Extension (fill the space with the rest)
$0b	1.b	 Attribute (%XLAD_VSHR. However, specifying something other than ASHR will naturally result in an error)
$0c	10.b The remainder of the file name of 8 bytes or more (the rest is filled with $ 00)
$16	1.w	Last Modified Time
		%mmmS_SSSS_HHHH_HMMM(H:時 Mm:分 S:秒)
$18	1.w	Last Modified Date
		%mmmD_DDDD_YYYY_YYYM(Y:年 Mm:月 D:日)
$1a	1.b	Special attribute change prohibition flag ┐ Used in BIND.X
$1b	1.b	Special attribute (%00A0_0SHR)            ┘
$1c	1.l	Offset from the beginning of the bind file

Total 32 ($20) bytes

  It is almost the same structure as the directory entry, so please be aware that the year, month, day, and hour byte of time are reversed like directory entries.
  The rest of the file is 0 bytes and the list ends. When it is 32 bytes or more, there is still continuation, otherwise it becomes abnormal structure.

・Structure of Z file header

offset	size
$00	1.w	第一識別子 $601a
$02	1.l	テキストセクションサイズ
$06	1.l	データセクションサイズ
$0a	1.l	ブロックストレージセクションサイズ(.comm .stack を含む)
$0e	8.b	予約(0)
$16	1.l	実行開始アドレス
$1a	1.w	第二識別子 $ffff

計 28($1c)バイト
```