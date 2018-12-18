```
Memory management pointer

offset  size
$00(0)  1.l     Previous memory management pointer (leading with 0)
$04(4)  1.l     Parent process memory management pointer (0 with no parent)
$08(8)  1.l     Last address of this memory block + 1
$0c(12) 1.l     Next memory management pointer (end with 0)

It is a structure of 16 bytes created at the head of each memory block.
The first memory block exists in the supervisor area inside the Human68k.

The most significant byte of the 4th byte (the process that secured memory)
represents the attribute of that memory block.

    $ff     Resident process (KEEP)
    $fe     unknown (MEMDRV)
    $fd     Sub memory block

Process management pointer

offset   size
$00(0)    4.l   (memory management pointer)
$10(16)   1.l   Environment's address (not secured if -1)
$14(20)   1.l   Return address at the end
$18(24)   1.l   Return address when interrupted by CTRL + C
$1c(28)   1.l   Return address when interrupt due to error
$20(32)   1.l   Command line address
$24(36)  12.b   File handler usage of process
                (In use = 1 in the order of bits 0 to 7 of $24 to $2f)
$30(48)   1.l   Start address of BSS
$34(52)   1.l   Start address of heap (same as BSS)
$38(56)   1.l   Initial stack address (end of heap + 1)
$3c(60)   1.l   USP of the parent process
$40(64)   1.l   Parent process SSP
$44(68)   1.w   Parent process SR
$46(70)   1.w   SR at abort
$48(72)   1.l   SSP at abort
$4c(76)   1.l   trap #10 vector
$50(80)   1.l   trap #11 vector
$54(84)   1.l   trap #12 vector
$58(88)   1.l   trap #13 vector
$5c(92)   1.l   trap #14 vector
$60(96)   1.l   shell activation flag (0: normal startup, -1: started as shell)
$64(100)  1.b   Module number
$65(101)  3.b   (unused)
$68(104)  1.l   Process management pointer of loaded child process
$6a(108)  5.l   (unused)
$80(128)  2.b   Drive name of executable file
$82(130) 66.b   Path name of executable file
$c4(196) 24.b   File name of executable file
$dc(220)  9.l   (unused)

A 256-byte structure (including a memory management pointer) for holding process information, which is created at the beginning of the memory block for each process.
```
