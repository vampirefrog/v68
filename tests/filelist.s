* DOS call files, sample of nfiles

.include doscall.mac
.include const.h

.text
.even

ent:
        lea.l mysp, sp * Initialization of sp
        bsr chkarg     * Command line analysis
        bsr do         * Main processing
        DOS _EXIT      * Successful completion

*
*
*

do:
        bsr chkname             * Preprocessing on file name

        move.w #ARCHIVE, -(sp)  * Search for the first file
        pea.l arg
        pea.l filbuf
        DOS _FILES
        lea.l 10(sp), sp

loop:   tst.l d0                * Did you find the file?
        bmi done                * If it is not found, processing is completed

        bsr setpath             * Reconfigure the obtained file name to the full path
        bsr doit                * Process one file
                                *
        pea.l filbuf            * Search for the following file
        DOS _NFILES
        addq.l #4, sp

        bra loop                * repeat

done: rts

                                * Process one file (just display the file name)

doit:
        pea.l arg               * It consists of setpath
        DOS _PRINT              * File name of full path
        addq.l #4, sp           * indicate

        pea.l crlfms            * Begin on a new line
        DOS _PRINT
        addq.l #4, sp

        rts

* Preprocess the file name before executing files

chkname:
        pea.l nambuf            * Expand file name
        pea.l arg
        DOS _NAMECK
        addq.l #8, sp

        tst.l d0                * if d0 <0
        bmi usage               * Invalid file name specification

        beq nowild              * if d 0 = 0, no wildcard specification
        cmpi.w #$00ff, d0
        bne wild

noname:
        lea.l arg, a0
        lea.l nambuf, a1
        bsr strcpy
        lea.l kome0, a1
        bsr strcpy

wild:

chknam0: rts

nowild:
        move.w #SUBDIR, -(sp)
        pea.l arg
        pea.l filbuf
        DOS _FILES
        lea.l 10(sp), sp

        tst.l d0
        bmi chknam0

        lea.l arg, a0
        lea.l komekome, a1
        bsr strcat

        bra chkname

setpath:
        lea.l arg, a0
        lea.l nambuf, a1
        bsr strcpy
        lea.l filbuf+30, a1
        bsr strcpy
        rts

chkarg:
        addq.l #1, a2
        bsr skipsp
*   tst.b (a2)
*   beq usage

        cmpi.b #'/', (a2)
        beq usage
        cmpi.b #'-', (a2)
        beq usage

        lea.l arg, a0
        bsr getarg

        bsr skipsp
        tst.b (a2)
        bne usage

        rts

getarg:
        move.l a0, -(sp)
gtarg0: tst.b (a2)
        beq gtarg1
        cmpi.b #SPACE, (a2)
        beq gtarg1
        cmpi.b #TAB, (a2)
        beq gtarg1
        cmpi.b #'-', (a2)
        beq gtarg1
        cmpi.b #'/', (a2)
        beq gtarg1
        move.b (a2)+, (a0)+
        bra gtarg0
gtarg1: clr.b (a0)
        movea.l (sp)+, a0
        rts

skpsp0: addq.l #1, a2

skipsp:
        cmpi.b #SPACE, (a2)
        beq sksp0
        cmpi.b #TAB, (a2)
        beq skpsp0
        rts

strcat:
        tst.b (a0)+
        bne strcat
        subq.l #1, a0
strcpy:
        move.b (a1)+, (a0)+
        bne strcpy
        subq.l #1, a0

        rts
* Application of process operation
usage:move. w #STDERR, -(sp)    * To standard error output
pea.l usgmes                    * Help message
DOS _FPUTS                      * Output
addq. I #6, sp                  * Stack compensation

move. w #1, -(sp）              * Have exit code 1
DOS _EXIT2                      * Error Exit

*
*   Message data
*
.data
.even

usgmes:
.dc.b 'Function: Displays the specified file name in full path'，CR, LF
.dc.b TAB,‘Wildcards can be used for file names', CR, LF
.dc.b 'Usage: FILELIST [file name]'
crlfms: .dc.b  CR, LF, 0
komekome:
.dc.b '\'
kome0: .dc.b '*.*', 0
*
*   Work area
*
.bss
.even

arg: .ds.b 256    * Bow "number cutting buffer
                                  * Put buffer used for files at even address
filbuf: .ds.b 53  * File information storage buffer
                                  * The buffer used by nameck may be an odd address
nambuf:. ds. b 91 * Buffer for expanding name of file

.stack
.even

mystack:
.ds.l 256         * Stack area
mysp:
. end
