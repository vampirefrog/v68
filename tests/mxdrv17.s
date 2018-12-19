;=============================================
;  Filename mxdrv17.x
;  Time Stamp Mon Dec 31 00:00:00 1979
;
;  Base address 000000
;  Exec address 0017ea
;  Text size    001ba6 byte(s)
;  Data size    000000 byte(s)
;  Bss  size    0006a2 byte(s)
;  449 Labels
;  Code Generate date Wed Dec 19 09:36:12 2018
;  Commandline H:/Code/v68/tests/dis/dis.x -a1 -C2 --overwrite --sp -m 68000 -b 2 -B -M -g mxdrv17.x mxdrv17.s
;          DIS version 3.14
;=============================================

        .include        H:/Code/v68/tests/dis/doscall.mac
        .include        H:/Code/v68/tests/dis/iocscall.mac
        .include        H:/Code/v68/tests/dis/fefunc.mac

        .cpu    68000

        .text

L000000:                                                        ;000000
        .dc.b   'EX17'                                          ;000000
L000004:                                                        ;000004
        .dc.b   'mxdrv206'                                      ;000004
L00000c:                                                        ;00000c
        movem.l d1-d7/a0-a6,-(sp)                               ;00000c
        lea.l   (L00220c,pc),a5                                 ;000010
        cmp.b   #$20,d0                 ;' '                    ;000014
        bcc.s   L000024                                         ;000018
        add.w   d0,d0                                           ;00001a
        move.w  (L00002a,pc,d0.w),d0                            ;00001c
        jsr     (L00002a,pc,d0.w)                               ;000020
L000024:                                                        ;000024
        movem.l (sp)+,d1-d7/a0-a6                               ;000024
        rte                                                     ;000028

L00002a:                                                        ;00002a
        .dc.w   L_FREE-L00002a                                  ;00002a
        .dc.w   L_ERROR-L00002a                                 ;00002c
        .dc.w   L_SETMDX-L00002a                                ;00002e
        .dc.w   L_SETPDX-L00002a                                ;000030
        .dc.w   L_PLAY-L00002a                                  ;000032
        .dc.w   L_STOP-L00002a                                  ;000034
        .dc.w   L_PAUSE-L00002a                                 ;000036
        .dc.w   L_CONT-L00002a                                  ;000038
        .dc.w   L_GetMDXTitle-L00002a                           ;00003a
        .dc.w   L_GetPDXFilename-L00002a                        ;00003c
        .dc.w   L_0A-L00002a                                    ;00003e
        .dc.w   L_0B-L00002a                                    ;000040
        .dc.w   L_FadeOut-L00002a                               ;000042
        .dc.w   L_0D-L00002a                                    ;000044
        .dc.w   L_SetChannelMask-L00002a                        ;000046
        .dc.w   L_PlayWithMask-L00002a                          ;000048
        .dc.w   L_GetOPMBuf-L00002a                             ;00004a
        .dc.w   L_11-L00002a                                    ;00004c
        .dc.w   L_GetPlaybackFlags-L00002a                      ;00004e
        .dc.w   L_SetIgnoreKeys-L00002a                         ;000050
        .dc.w   L_14-L00002a                                    ;000052
        .dc.w   L_15-L00002a                                    ;000054
        .dc.w   L_16-L00002a                                    ;000056
        .dc.w   L_17-L00002a                                    ;000058
        .dc.w   L_GetPCMBuf-L00002a                             ;00005a
        .dc.w   L_19-L00002a                                    ;00005c
        .dc.w   L_1A-L00002a                                    ;00005e
        .dc.w   L_1B-L00002a                                    ;000060
        .dc.w   L_1C-L00002a                                    ;000062
        .dc.w   L_1D-L00002a                                    ;000064
        .dc.w   L_1E-L00002a                                    ;000066
        .dc.w   L_1F-L00002a                                    ;000068
L_0A:                                                           ;00006a
        move.b  d1,(-$03f8,a5)                                  ;00006a
        rts                                                     ;00006e

L_0B:                                                           ;000070
        move.b  d1,(-$03f7,a5)                                  ;000070
        rts                                                     ;000074

L_FadeOut:                                                      ;000076
        move.w  d1,(-$03ee,a5)                                  ;000076
        st.b    (-$03f5,a5)                                     ;00007a
L00007e:                                                        ;00007e
        rts                                                     ;00007e

L_SetChannelMask:                                               ;000080
        move.w  d1,(-$03f0,a5)                                  ;000080
        rts                                                     ;000084

L_GetOPMBuf:                                                    ;000086
        lea.l   (L001e3c,pc),a0                                 ;000086
        move.l  a0,d0                                           ;00008a
        rts                                                     ;00008c

L_11:                                                           ;00008e
        lea.l   (L001e0e,pc),a0                                 ;00008e
        tst.l   d1                                              ;000092
        bmi.s   L00009a                                         ;000094
        move.b  d1,(a0)                                         ;000096
        rts                                                     ;000098

L00009a:                                                        ;00009a
        move.b  (a0),d0                                         ;00009a
        rts                                                     ;00009c

L_GetPlaybackFlags:                                             ;00009e
        move.b  (-$03fa,a5),-(sp)                               ;00009e
        move.w  (sp)+,d0                                        ;0000a2
        move.b  (-$03f9,a5),d0                                  ;0000a4
        rts                                                     ;0000a8

L_SetIgnoreKeys:                                                ;0000aa
        move.b  (-$0402,a5),d0                                  ;0000aa
        move.b  d1,(-$0402,a5)                                  ;0000ae
        rts                                                     ;0000b2

L_14:                                                           ;0000b4
        move.w  (-$0406,a5),d0                                  ;0000b4
        not.w   d0                                              ;0000b8
        rts                                                     ;0000ba

L_15:                                                           ;0000bc
        move.b  (-$0401,a5),d0                                  ;0000bc
        move.b  d1,(-$0401,a5)                                  ;0000c0
        rts                                                     ;0000c4

L_16:                                                           ;0000c6
        move.b  (-$0404,a5),d0                                  ;0000c6
        move.b  d1,(-$0404,a5)                                  ;0000ca
        bsr.w   L_STOP                                          ;0000ce
        rts                                                     ;0000d2

L_17:                                                           ;0000d4
        move.b  (-$0404,a5),d0                                  ;0000d4
        beq.w   L0001ee                                         ;0000d8
L0000dc:                                                        ;0000dc
        movem.l d1-d7/a0-a6,-(sp)                               ;0000dc
        st.b    ($0039,a5)                                      ;0000e0
        lea.l   (L001e1e,pc),a0                                 ;0000e4
        lea.l   (fadeout_enable,pc),a1                          ;0000e8
        tst.b   (a1)                                            ;0000ec
        beq.w   L000174                                         ;0000ee
        bpl.s   L0000fc                                         ;0000f2
        move.b  #$7f,(a1)                                       ;0000f4
        move.w  (a0),($0002,a0)                                 ;0000f8
L0000fc:                                                        ;0000fc
        tst.w   ($0002,a0)                                      ;0000fc
        bmi.s   L000108                                         ;000100
        subq.w  #2,($0002,a0)                                   ;000102
        bra.s   L000174                                         ;000106

L000108:                                                        ;000108
        lea.l   (fadeout_offset,pc),a1                          ;000108
        cmpi.b  #$0a,(a1)                                       ;00010c
        bge.s   L000120                                         ;000110
L000112:                                                        ;000112
        cmpi.b  #$3e,(a1)               ;'>'                    ;000112
        bge.s   L000126                                         ;000116
        addq.b  #1,(a1)                                         ;000118
        move.w  (a0),($0002,a0)                                 ;00011a
        bra.s   L000174                                         ;00011e

L000120:                                                        ;000120
        st.b    (-$03f7,a5)                                     ;000120
        bra.s   L000112                                         ;000124

L000126:                                                        ;000126
        tst.b   (-$03f4,a5)                                     ;000126
        beq.s   L000134                                         ;00012a
        bsr.w   L00077a                                         ;00012c
        bra.w   L0001d6                                         ;000130

L000134:                                                        ;000134
        move.b  #$7f,(a1)                                       ;000134
        clr.b   (-$03f5,a5)                                     ;000138
        move.b  #$01,(-$03f9,a5)                                ;00013c
        bsr.w   L_PAUSE                                         ;000142
        movea.l ($0088),a0                                      ;000146
        move.l  (-$0008,a0),d0                                  ;00014a
        cmp.l   #$50434d34,d0           ;'PCM4'                 ;00014e
        beq.s   L00015e                                         ;000154
        cmp.l   #$50434d38,d0           ;'PCM8'                 ;000156
        bne.s   L000164                                         ;00015c
L00015e:                                                        ;00015e
        move.w  #$0100,d0                                       ;00015e
        trap    #2                                              ;000162
L000164:                                                        ;000164
        tst.b   (-$0418,a5)                                     ;000164
        beq.s   L000174                                         ;000168
        move.w  #$01ff,d0                                       ;00016a
        trap    #2                                              ;00016e
        clr.b   (-$0418,a5)                                     ;000170
L000174:                                                        ;000174
        lea.l   (opm_tempo,pc),a0                               ;000174
        move.b  (a0),d2                                         ;000178
        moveq.l #$12,d1                                         ;00017a
        tst.b   (-$03f9,a5)                                     ;00017c
        bne.s   L0001d6                                         ;000180
        addq.w  #1,(-$0666,a5)                                  ;000182
        lea.l   (L001f3c,pc),a6                                 ;000186
        moveq.l #$00,d7                                         ;00018a
L00018c:                                                        ;00018c
        bsr.w   L001050                                         ;00018c
        bsr.w   L0011b4                                         ;000190
        move.w  (channel_mask,pc),d0                            ;000194
        btst.l  d7,d0                                           ;000198
        bne.s   L0001a0                                         ;00019a
        bsr.w   L000c66                                         ;00019c
L0001a0:                                                        ;0001a0
        lea.l   ($0050,a6),a6                                   ;0001a0
        addq.w  #1,d7                                           ;0001a4
        cmp.w   #$0009,d7                                       ;0001a6
        bcs.s   L00018c                                         ;0001aa
        tst.b   (-$0418,a5)                                     ;0001ac
        beq.s   L0001d6                                         ;0001b0
        lea.l   (L001bc4,pc),a6                                 ;0001b2
L0001b6:                                                        ;0001b6
        bsr.w   L001050                                         ;0001b6
        bsr.w   L0011b4                                         ;0001ba
        move.w  (channel_mask,pc),d0                            ;0001be
        btst.l  d7,d0                                           ;0001c2
        bne.s   L0001ca                                         ;0001c4
        bsr.w   L000c66                                         ;0001c6
L0001ca:                                                        ;0001ca
        lea.l   ($0050,a6),a6                                   ;0001ca
        addq.w  #1,d7                                           ;0001ce
        cmp.w   #$0010,d7                                       ;0001d0
        bcs.s   L0001b6                                         ;0001d4
L0001d6:                                                        ;0001d6
        tst.b   ($00e90003)                                     ;0001d6
        bmi.s   L0001d6                                         ;0001dc
        move.b  #$1b,($00e90001)                                ;0001de
        clr.b   ($0039,a5)                                      ;0001e6
        movem.l (sp)+,d1-d7/a0-a6                               ;0001ea
L0001ee:                                                        ;0001ee
        move.b  (-$03fa,a5),-(sp)                               ;0001ee
        move.w  (sp)+,d0                                        ;0001f2
        move.b  (-$03f9,a5),d0                                  ;0001f4
        rts                                                     ;0001f8

L_GetPCMBuf:                                                    ;0001fa
        lea.l   (L001bc4,pc),a0                                 ;0001fa
        move.l  a0,d0                                           ;0001fe
        rts                                                     ;000200

L_19:                                                           ;000202
        lea.l   (pcm_work_area,pc),a0                           ;000202
        move.l  a0,d0                                           ;000206
        rts                                                     ;000208

L_1A:                                                           ;00020a
        bsr.s   L000216                                         ;00020a
        tst.l   d0                                              ;00020c
        bmi.s   L000214                                         ;00020e
        move.l  d1,($0004,sp)                                   ;000210
L000214:                                                        ;000214
        rts                                                     ;000214

L000216:                                                        ;000216
        movem.l d2-d4/a0-a2,-(sp)                               ;000216
        movea.l a0,a1                                           ;00021a
        moveq.l #$00,d0                                         ;00021c
        moveq.l #$ff,d3                                         ;00021e
        moveq.l #$00,d1                                         ;000220
L000222:                                                        ;000222
        move.l  (a1)+,d4                                        ;000222
        move.l  (a1)+,d2                                        ;000224
        and.l   #$00ffffff,d4                                   ;000226
        beq.s   L00025a                                         ;00022c
        cmp.l   (-$0008,a1),d4                                  ;00022e
        bne.s   L000260                                         ;000232
        and.l   #$00ffffff,d2                                   ;000234
        beq.s   L00025a                                         ;00023a
        cmp.l   (-$0004,a1),d2                                  ;00023c
        bne.s   L000260                                         ;000240
        add.l   d4,d2                                           ;000242
        cmp.l   d1,d2                                           ;000244
        bcs.s   L00024a                                         ;000246
        move.l  d2,d1                                           ;000248
L00024a:                                                        ;00024a
        cmp.l   d4,d3                                           ;00024a
        bcs.s   L000250                                         ;00024c
        move.l  d4,d3                                           ;00024e
L000250:                                                        ;000250
        lea.l   (a0,d3.l),a2                                    ;000250
        cmpa.l  a2,a1                                           ;000254
        beq.s   L00025e                                         ;000256
        bhi.s   L000266                                         ;000258
L00025a:                                                        ;00025a
        addq.w  #1,d0                                           ;00025a
        bra.s   L000222                                         ;00025c

L00025e:                                                        ;00025e
        addq.w  #1,d0                                           ;00025e
L000260:                                                        ;000260
        movem.l (sp)+,d2-d4/a0-a2                               ;000260
        rts                                                     ;000264

L000266:                                                        ;000266
        moveq.l #$ff,d0                                         ;000266
        movem.l (sp)+,d2-d4/a0-a2                               ;000268
        rts                                                     ;00026c

L_1B:                                                           ;00026e
        movem.l d1-d5/a0-a2,-(sp)                               ;00026e
        bsr.s   L000216                                         ;000272
        move.l  d0,d2                                           ;000274
        bmi.s   L0002e4                                         ;000276
        move.l  d0,d5                                           ;000278
        lsl.l   #3,d0                                           ;00027a
        moveq.l #$60,d3                 ;'`'                    ;00027c
L00027e:                                                        ;00027e
        sub.l   d3,d2                                           ;00027e
        bcc.s   L00027e                                         ;000280
        add.l   d3,d2                                           ;000282
        beq.s   L0002e4                                         ;000284
        sub.l   d2,d3                                           ;000286
        move.l  d3,d4                                           ;000288
        lsl.l   #3,d3                                           ;00028a
        move.l  d1,d2                                           ;00028c
        addq.l  #1,d2                                           ;00028e
        and.w   #$fffe,d2                                       ;000290
        lea.l   (a0,d2.l),a2                                    ;000294
        add.l   d3,d1                                           ;000298
        lea.l   (a0,d1.l),a1                                    ;00029a
        sub.l   d0,d2                                           ;00029e
        lsr.l   #1,d2                                           ;0002a0
        move.l  d2,d0                                           ;0002a2
        lsr.l   #1,d0                                           ;0002a4
        subq.l  #1,d0                                           ;0002a6
        swap.w  d0                                              ;0002a8
L0002aa:                                                        ;0002aa
        swap.w  d0                                              ;0002aa
L0002ac:                                                        ;0002ac
        move.l  -(a2),-(a1)                                     ;0002ac
        dbra.w  d0,L0002ac                                      ;0002ae
        swap.w  d0                                              ;0002b2
        dbra.w  d0,L0002aa                                      ;0002b4
        and.w   #$0001,d2                                       ;0002b8
        beq.s   L0002c0                                         ;0002bc
        move.w  -(a2),-(a1)                                     ;0002be
L0002c0:                                                        ;0002c0
        subq.w  #1,d4                                           ;0002c0
L0002c2:                                                        ;0002c2
        clr.l   -(a1)                                           ;0002c2
        clr.l   -(a1)                                           ;0002c4
        dbra.w  d4,L0002c2                                      ;0002c6
        subq.l  #1,d5                                           ;0002ca
        swap.w  d5                                              ;0002cc
L0002ce:                                                        ;0002ce
        swap.w  d5                                              ;0002ce
L0002d0:                                                        ;0002d0
        move.l  (a0)+,d0                                        ;0002d0
        beq.s   L0002d8                                         ;0002d2
        add.l   d3,(-$0004,a0)                                  ;0002d4
L0002d8:                                                        ;0002d8
        addq.w  #4,a0                                           ;0002d8
        dbra.w  d5,L0002d0                                      ;0002da
        swap.w  d5                                              ;0002de
        dbra.w  d5,L0002ce                                      ;0002e0
L0002e4:                                                        ;0002e4
        move.l  d1,d0                                           ;0002e4
        movem.l (sp)+,d1-d5/a0-a2                               ;0002e6
        rts                                                     ;0002ea

L_1C:                                                           ;0002ec
        movem.l d1-d7/a0-a4,-(sp)                               ;0002ec
        bsr.w   L000216                                         ;0002f0
        tst.l   d0                                              ;0002f4
        bmi.w   L000462                                         ;0002f6
        add.l   a0,d1                                           ;0002fa
        addq.l  #1,d1                                           ;0002fc
        and.w   #$fffe,d1                                       ;0002fe
        move.l  d1,d3                                           ;000302
        move.l  d3,d7                                           ;000304
        move.l  d0,d2                                           ;000306
        exg.l   a0,a1                                           ;000308
        bsr.w   L000216                                         ;00030a
        tst.l   d0                                              ;00030e
        bmi.w   L000462                                         ;000310
        add.l   d1,d3                                           ;000314
        add.l   a0,d1                                           ;000316
        move.l  d1,d6                                           ;000318
        move.l  d0,d1                                           ;00031a
        lsl.l   #3,d1                                           ;00031c
        add.l   a0,d1                                           ;00031e
        move.l  d1,d4                                           ;000320
        suba.l  d3,a2                                           ;000322
        bcs.w   L00045a                                         ;000324
        moveq.l #$60,d1                 ;'`'                    ;000328
        move.l  d2,d3                                           ;00032a
L00032c:                                                        ;00032c
        sub.l   d1,d3                                           ;00032c
        bcc.s   L00032c                                         ;00032e
        add.l   d1,d3                                           ;000330
        beq.s   L000342                                         ;000332
        sub.l   d1,d3                                           ;000334
        neg.l   d3                                              ;000336
        move.l  d3,d1                                           ;000338
        lsl.l   #3,d3                                           ;00033a
        cmp.l   a2,d3                                           ;00033c
        bhi.w   L00045a                                         ;00033e
L000342:                                                        ;000342
        add.l   d0,d3                                           ;000342
        lsl.l   #3,d3                                           ;000344
        add.l   d7,d3                                           ;000346
        movea.l a0,a4                                           ;000348
        cmp.l   a0,d3                                           ;00034a
        bcs.s   L00037a                                         ;00034c
        move.l  d0,d1                                           ;00034e
        lsl.l   #3,d1                                           ;000350
        cmp.l   (-$0664,a5),d1                                  ;000352
        bhi.w   L00045e                                         ;000356
        movea.l (-$0660,a5),a4                                  ;00035a
        movea.l a0,a3                                           ;00035e
        move.l  d0,d1                                           ;000360
        subq.l  #1,d1                                           ;000362
        swap.w  d1                                              ;000364
L000366:                                                        ;000366
        swap.w  d1                                              ;000366
L000368:                                                        ;000368
        move.l  (a3)+,(a4)+                                     ;000368
        move.l  (a3)+,(a4)+                                     ;00036a
        dbra.w  d1,L000368                                      ;00036c
        swap.w  d1                                              ;000370
        dbra.w  d1,L000366                                      ;000372
        movea.l (-$0660,a5),a4                                  ;000376
L00037a:                                                        ;00037a
        lsl.l   #3,d0                                           ;00037a
        move.l  d0,d5                                           ;00037c
        exg.l   a0,a1                                           ;00037e
        bsr.w   L_1B                                            ;000380
        tst.l   d0                                              ;000384
        bmi.w   L000466                                         ;000386
        bsr.w   L000216                                         ;00038a
        move.l  d0,d2                                           ;00038e
        bmi.w   L000462                                         ;000390
        add.l   a0,d1                                           ;000394
        addq.l  #1,d1                                           ;000396
        and.w   #$fffe,d1                                       ;000398
        movea.l d1,a2                                           ;00039c
        add.l   d5,d1                                           ;00039e
        lsl.l   #3,d0                                           ;0003a0
        add.l   a0,d0                                           ;0003a2
        add.l   d5,d0                                           ;0003a4
        movea.l d1,a3                                           ;0003a6
        movea.l a3,a1                                           ;0003a8
        sub.l   d0,d1                                           ;0003aa
        move.l  d1,d7                                           ;0003ac
        lsr.l   #2,d1                                           ;0003ae
        move.w  sr,-(sp)                                        ;0003b0
        subq.l  #1,d1                                           ;0003b2
        swap.w  d1                                              ;0003b4
L0003b6:                                                        ;0003b6
        swap.w  d1                                              ;0003b6
L0003b8:                                                        ;0003b8
        move.l  -(a2),-(a3)                                     ;0003b8
        dbra.w  d1,L0003b8                                      ;0003ba
        swap.w  d1                                              ;0003be
        dbra.w  d1,L0003b6                                      ;0003c0
        move.w  (sp)+,sr                                        ;0003c4
        bcc.s   L0003ca                                         ;0003c6
        move.w  -(a2),-(a3)                                     ;0003c8
L0003ca:                                                        ;0003ca
        movea.l d0,a2                                           ;0003ca
        suba.l  d5,a2                                           ;0003cc
        cmpa.l  a2,a4                                           ;0003ce
        beq.s   L0003ea                                         ;0003d0
        move.l  d5,d1                                           ;0003d2
        lsr.l   #3,d1                                           ;0003d4
        subq.l  #1,d1                                           ;0003d6
        swap.w  d1                                              ;0003d8
L0003da:                                                        ;0003da
        swap.w  d1                                              ;0003da
L0003dc:                                                        ;0003dc
        move.l  (a4)+,(a2)+                                     ;0003dc
        move.l  (a4)+,(a2)+                                     ;0003de
        dbra.w  d1,L0003dc                                      ;0003e0
        swap.w  d1                                              ;0003e4
        dbra.w  d1,L0003da                                      ;0003e6
L0003ea:                                                        ;0003ea
        movea.l d4,a2                                           ;0003ea
        sub.l   d4,d6                                           ;0003ec
        move.l  d6,d1                                           ;0003ee
        lsr.l   #2,d1                                           ;0003f0
        subq.l  #1,d1                                           ;0003f2
        swap.w  d1                                              ;0003f4
L0003f6:                                                        ;0003f6
        swap.w  d1                                              ;0003f6
L0003f8:                                                        ;0003f8
        move.l  (a2)+,(a1)+                                     ;0003f8
        dbra.w  d1,L0003f8                                      ;0003fa
        swap.w  d1                                              ;0003fe
        dbra.w  d1,L0003f6                                      ;000400
        move.w  d6,d1                                           ;000404
        and.w   #$0002,d1                                       ;000406
        beq.s   L00040e                                         ;00040a
        move.w  (a2)+,(a1)+                                     ;00040c
L00040e:                                                        ;00040e
        and.w   #$0001,d6                                       ;00040e
        beq.s   L000416                                         ;000412
        move.b  (a2)+,(a1)+                                     ;000414
L000416:                                                        ;000416
        suba.l  a0,a1                                           ;000416
        move.l  d5,d1                                           ;000418
        move.l  d2,d0                                           ;00041a
        lsl.l   #3,d0                                           ;00041c
        add.l   d0,d7                                           ;00041e
        subq.l  #1,d2                                           ;000420
        swap.w  d2                                              ;000422
L000424:                                                        ;000424
        swap.w  d2                                              ;000424
L000426:                                                        ;000426
        move.l  (a0),d0                                         ;000426
        beq.s   L00042c                                         ;000428
        add.l   d1,(a0)                                         ;00042a
L00042c:                                                        ;00042c
        addq.w  #8,a0                                           ;00042c
        dbra.w  d2,L000426                                      ;00042e
        swap.w  d2                                              ;000432
        dbra.w  d2,L000424                                      ;000434
        lsr.l   #3,d5                                           ;000438
        subq.l  #1,d5                                           ;00043a
        swap.w  d5                                              ;00043c
L00043e:                                                        ;00043e
        swap.w  d5                                              ;00043e
L000440:                                                        ;000440
        move.l  (a0),d0                                         ;000440
        beq.s   L000446                                         ;000442
        add.l   d7,(a0)                                         ;000444
L000446:                                                        ;000446
        addq.w  #8,a0                                           ;000446
        dbra.w  d5,L000440                                      ;000448
        swap.w  d5                                              ;00044c
        dbra.w  d5,L00043e                                      ;00044e
        move.l  a1,d0                                           ;000452
L000454:                                                        ;000454
        movem.l (sp)+,d1-d7/a0-a4                               ;000454
        rts                                                     ;000458

L00045a:                                                        ;00045a
        moveq.l #$ff,d0                                         ;00045a
        bra.s   L000454                                         ;00045c

L00045e:                                                        ;00045e
        moveq.l #$fe,d0                                         ;00045e
        bra.s   L000454                                         ;000460

L000462:                                                        ;000462
        moveq.l #$fd,d0                                         ;000462
        bra.s   L000454                                         ;000464

L000466:                                                        ;000466
        moveq.l #$fc,d0                                         ;000466
        bra.s   L000454                                         ;000468

L_1D:                                                           ;00046a
        move.b  (-$0404,a5),d4                                  ;00046a
        move.w  d1,d3                                           ;00046e
        st.b    (-$0404,a5)                                     ;000470
        move.w  #$ffff,d1                                       ;000474
        movem.l d2-d4,-(sp)                                     ;000478
        bsr.w   L_PlayWithMask                                  ;00047c
        movem.l (sp)+,d2-d4                                     ;000480
        bra.s   L000496                                         ;000484

L_1E:                                                           ;000486
        move.b  (-$0404,a5),d4                                  ;000486
        move.w  d1,d3                                           ;00048a
        move.w  #$ffff,(-$03f0,a5)                              ;00048c
        st.b    (-$0404,a5)                                     ;000492
L000496:                                                        ;000496
        ori.w   #$0700,sr                                       ;000496
        andi.b  #$f7,($00e88009)                                ;00049a
        andi.w  #$f8ff,sr                                       ;0004a2
        tst.b   (-$03f9,a5)                                     ;0004a6
        bne.s   L0004c0                                         ;0004aa
        subq.w  #1,d2                                           ;0004ac
        bcs.s   L0004c0                                         ;0004ae
        movem.l d2-d4,-(sp)                                     ;0004b0
L0004b4:                                                        ;0004b4
        bsr.w   L0000dc                                         ;0004b4
        dbra.w  d2,L0004b4                                      ;0004b8
        movem.l (sp)+,d2-d4                                     ;0004bc
L0004c0:                                                        ;0004c0
        move.w  d3,(-$03f0,a5)                                  ;0004c0
        move.b  d4,(-$0404,a5)                                  ;0004c4
        bne.s   L_1F                                            ;0004c8
        tst.b   (-$03f9,a5)                                     ;0004ca
        bne.s   L_1F                                            ;0004ce
        bsr.w   L00056a                                         ;0004d0
        moveq.l #$12,d1                                         ;0004d4
        move.b  (-$0400,a5),d2                                  ;0004d6
        bsr.w   L_WRITEOPM                                      ;0004da
        moveq.l #$14,d1                                         ;0004de
        moveq.l #$3a,d2                 ;':'                    ;0004e0
        bsr.w   L_WRITEOPM                                      ;0004e2
L_1F:                                                           ;0004e6
        move.w  (-$0666,a5),d0                                  ;0004e6
        rts                                                     ;0004ea

L_0D:                                                           ;0004ec
        cmp.b   #$f0,d1                                         ;0004ec
        beq.s   L000552                                         ;0004f0
        cmp.b   #$fc,d1                                         ;0004f2
        beq.s   L00052e                                         ;0004f6
        tst.l   d1                                              ;0004f8
        bmi.s   L000534                                         ;0004fa
        tst.b   (-$03f4,a5)                                     ;0004fc
        bne.w   L_ERROR                                         ;000500
        move.l  a2,(-$03dc,a5)                                  ;000504
        move.l  a1,(-$03e8,a5)                                  ;000508
        move.l  a1,(-$03e4,a5)                                  ;00050c
L000510:                                                        ;000510
        tst.w   (a1)                                            ;000510
        beq.s   L000518                                         ;000512
        addq.w  #6,a1                                           ;000514
        bra.s   L000510                                         ;000516

L000518:                                                        ;000518
        subq.w  #6,a1                                           ;000518
        move.l  a1,(-$03e0,a5)                                  ;00051a
        st.b    (-$03f4,a5)                                     ;00051e
        st.b    (-$03f3,a5)                                     ;000522
        movea.l (L001e24,pc),a0                                 ;000526
        bra.w   L000788                                         ;00052a

L00052e:                                                        ;00052e
        move.b  (L001e19,pc),d0                                 ;00052e
        rts                                                     ;000532

L000534:                                                        ;000534
        clr.b   (-$03f4,a5)                                     ;000534
        clr.b   ($0024,a5)                                      ;000538
        clr.b   ($0025,a5)                                      ;00053c
        movea.l (mdx_data,pc),a0                                ;000540
        move.l  (a0),($000c,a5)                                 ;000544
        move.l  ($0004,a0),($0010,a5)                           ;000548
        bra.w   StopPlayback                                    ;00054e

L000552:                                                        ;000552
        bsr.s   L000534                                         ;000552
L000554:                                                        ;000554
        movea.l (L001e30,pc),a0                                 ;000554
        pea.l   (a0)                                            ;000558
        DOS     _MFREE                                          ;00055a
        addq.w  #4,sp                                           ;00055c
        moveq.l #$00,d0                                         ;00055e
        move.l  d0,(-$03dc,a5)                                  ;000560
        move.b  d0,(-$03f3,a5)                                  ;000564
        rts                                                     ;000568

L00056a:                                                        ;00056a
        move.w  sr,-(sp)                                        ;00056a
        ori.w   #$0700,sr                                       ;00056c
        clr.b   (-$03f9,a5)                                     ;000570
        tst.b   (-$0404,a5)                                     ;000574
        bne.s   L000596                                         ;000578
        lea.l   (L_OPMINT,pc),a1                                ;00057a
        suba.l  a0,a0                                           ;00057e
        move.l  a1,($010c,a0)                                   ;000580
        movea.l #$00e88000,a0                                   ;000584
        ori.b   #$08,($0009,a0)                                 ;00058a
        ori.b   #$08,($0015,a0)                                 ;000590
L000596:                                                        ;000596
        move.w  (sp)+,sr                                        ;000596
        rts                                                     ;000598

L_FREE:                                                         ;00059a
        bsr.w   StopPlayback                                    ;00059a
        move.l  (a5),($0090,a0)                                 ;00059e
        pea.l   (L000000-$0000f0,pc)                            ;0005a2
        DOS     _MFREE                                          ;0005a6
        addq.w  #4,sp                                           ;0005a8
        tst.b   (-$03f3,a5)                                     ;0005aa
        beq.s   L0005b2                                         ;0005ae
        bra.s   L000554                                         ;0005b0

L0005b2:                                                        ;0005b2
        rts                                                     ;0005b2

L_SETMDX:                                                       ;0005b4
        tst.b   (-$03f4,a5)                                     ;0005b4
        beq.s   L0005c4                                         ;0005b8
        movem.l d1/a1,-(sp)                                     ;0005ba
        bsr.s   L000552                                         ;0005be
        movem.l (sp)+,d1/a1                                     ;0005c0
L0005c4:                                                        ;0005c4
        lea.l   ($0024,a5),a2                                   ;0005c4
        movea.l (mdx_data,pc),a0                                ;0005c8
        move.l  a0,($000c,a5)                                   ;0005cc
        move.l  ($0014,a5),d0                                   ;0005d0
        bra.s   L0005f8                                         ;0005d4

L_SETPDX:                                                       ;0005d6
        tst.b   (-$03f4,a5)                                     ;0005d6
        beq.s   L0005e8                                         ;0005da
        movem.l d1/a1,-(sp)                                     ;0005dc
        bsr.w   L000552                                         ;0005e0
        movem.l (sp)+,d1/a1                                     ;0005e4
L0005e8:                                                        ;0005e8
        lea.l   ($0025,a5),a2                                   ;0005e8
        movea.l (pdx_data,pc),a0                                ;0005ec
        move.l  a0,($0010,a5)                                   ;0005f0
        move.l  ($0018,a5),d0                                   ;0005f4
L0005f8:                                                        ;0005f8
        cmp.l   d1,d0                                           ;0005f8
        bcs.s   L000630                                         ;0005fa
        movem.l d1/a0-a2,-(sp)                                  ;0005fc
        bsr.s   StopPlayback                                    ;000600
        movem.l (sp)+,d1/a0-a2                                  ;000602
        move.w  d1,d0                                           ;000606
        andi.w  #$0003,d0                                       ;000608
        lsr.l   #2,d1                                           ;00060c
        swap.w  d1                                              ;00060e
L000610:                                                        ;000610
        swap.w  d1                                              ;000610
L000612:                                                        ;000612
        move.l  (a1)+,(a0)+                                     ;000612
        dbra.w  d1,L000612                                      ;000614
        swap.w  d1                                              ;000618
        dbra.w  d1,L000610                                      ;00061a
        tst.w   d0                                              ;00061e
        beq.s   L00062a                                         ;000620
        subq.w  #1,d0                                           ;000622
L000624:                                                        ;000624
        move.b  (a1)+,(a0)+                                     ;000624
        dbra.w  d0,L000624                                      ;000626
L00062a:                                                        ;00062a
        st.b    (a2)                                            ;00062a
        moveq.l #$00,d0                                         ;00062c
        rts                                                     ;00062e

L000630:                                                        ;000630
        bset.l  #$1f,d0                                         ;000630
        rts                                                     ;000634

L_STOP:                                                         ;000636
        tst.b   (-$03f4,a5)                                     ;000636
        bne.w   L000552                                         ;00063a
StopPlayback:                                                   ;00063e
        move.b  #$01,(-$03f9,a5)                                ;00063e
        move.w  sr,-(sp)                                        ;000644
        ori.w   #$0700,sr                                       ;000646
        bsr.s   PausePlayback                                   ;00064a
        movea.l ($0088),a0                                      ;00064c
        move.l  (-$0008,a0),d0                                  ;000650
        cmp.l   #$50434d34,d0           ;'PCM4'                 ;000654
        beq.s   L000664                                         ;00065a
        cmp.l   #$50434d38,d0           ;'PCM8'                 ;00065c
        bne.s   L00066a                                         ;000662
L000664:                                                        ;000664
        move.w  #$0100,d0                                       ;000664
        trap    #2                                              ;000668
L00066a:                                                        ;00066a
        tst.b   (-$0418,a5)                                     ;00066a
        beq.s   L00067a                                         ;00066e
        move.w  #$01ff,d0                                       ;000670
        trap    #2                                              ;000674
        clr.b   (-$0418,a5)                                     ;000676
L00067a:                                                        ;00067a
        moveq.l #$0f,d2                                         ;00067a
        moveq.l #$e0,d1                                         ;00067c
L00067e:                                                        ;00067e
        bsr.w   L_WRITEOPM                                      ;00067e
        addq.b  #1,d1                                           ;000682
        bne.s   L00067e                                         ;000684
        lea.l   (L00223c,pc),a0                                 ;000686
        lea.l   (pcm_work_area,pc),a1                           ;00068a
        moveq.l #$07,d3                                         ;00068e
        moveq.l #$00,d2                                         ;000690
        moveq.l #$08,d1                                         ;000692
L000694:                                                        ;000694
        bsr.w   L_WRITEOPM                                      ;000694
        move.b  d2,(a0)+                                        ;000698
        move.b  d2,(a1)+                                        ;00069a
        addq.b  #1,d2                                           ;00069c
        dbra.w  d3,L000694                                      ;00069e
        movea.l #$00e88000,a0                                   ;0006a2
        andi.b  #$f7,($0009,a0)                                 ;0006a8
        andi.b  #$f7,($0015,a0)                                 ;0006ae
        suba.l  a0,a0                                           ;0006b4
        move.l  ($0004,a5),($010c,a0)                           ;0006b6
        move.w  (sp)+,sr                                        ;0006bc
        rts                                                     ;0006be

L_PAUSE:                                                        ;0006c0
        st.b    (-$03fa,a5)                                     ;0006c0
PausePlayback:                                                  ;0006c4
        moveq.l #$07,d7                                         ;0006c4
        lea.l   (L001f3c,pc),a6                                 ;0006c6
L0006ca:                                                        ;0006ca
        moveq.l #$7f,d0                                         ;0006ca
        bsr.w   WriteTL                                         ;0006cc
        lea.l   ($0050,a6),a6                                   ;0006d0
        dbra.w  d7,L0006ca                                      ;0006d4
        movea.l ($0088),a0                                      ;0006d8
        move.l  (-$0008,a0),d0                                  ;0006dc
        cmp.l   #$50434d34,d0           ;'PCM4'                 ;0006e0
        beq.s   L0006f0                                         ;0006e6
        cmp.l   #$50434d38,d0           ;'PCM8'                 ;0006e8
        bne.s   L00070c                                         ;0006ee
L0006f0:                                                        ;0006f0
        move.w  #$01fc,d0                                       ;0006f0
        moveq.l #$ff,d1                                         ;0006f4
        trap    #2                                              ;0006f6
        cmp.b   #$01,d0                                         ;0006f8
        bne.s   L000706                                         ;0006fc
        move.w  #$0101,d0                                       ;0006fe
        trap    #2                                              ;000702
        rts                                                     ;000704

L000706:                                                        ;000706
        moveq.l #$67,d0                 ;'g'                    ;000706
        moveq.l #$01,d1                                         ;000708
        trap    #15                                             ;00070a
L00070c:                                                        ;00070c
        moveq.l #$67,d0                 ;'g'                    ;00070c
        moveq.l #$00,d1                                         ;00070e
        trap    #15                                             ;000710
        rts                                                     ;000712

L_CONT:                                                         ;000714
        clr.b   (-$03fa,a5)                                     ;000714
        moveq.l #$07,d7                                         ;000718
        lea.l   (L001f3c,pc),a6                                 ;00071a
L00071e:                                                        ;00071e
        bsr.w   SetOPMVolume                                    ;00071e
        lea.l   ($0050,a6),a6                                   ;000722
        dbra.w  d7,L00071e                                      ;000726
        movea.l ($0088),a0                                      ;00072a
        move.l  (-$0008,a0),d0                                  ;00072e
        cmp.l   #$50434d34,d0           ;'PCM4'                 ;000732
        beq.s   L000742                                         ;000738
        cmp.l   #$50434d38,d0           ;'PCM8'                 ;00073a
        bne.s   EnableTimerIRQ                                  ;000740
L000742:                                                        ;000742
        move.w  #$01fc,d0                                       ;000742
        moveq.l #$ff,d1                                         ;000746
        trap    #2                                              ;000748
        cmp.b   #$01,d0                                         ;00074a
        bne.s   EnableTimerIRQ                                  ;00074e
        move.w  #$0102,d0                                       ;000750
        trap    #2                                              ;000754
EnableTimerIRQ:                                                 ;000756
        moveq.l #$30,d2                 ;'0'                    ;000756
        move.b  (disable_timer_writes,pc),d1                    ;000758
        bne.s   L000760                                         ;00075c
        moveq.l #$3a,d2                 ;':'                    ;00075e
L000760:                                                        ;000760
        moveq.l #$14,d1                                         ;000760
        bra.w   L_WRITEOPM                                      ;000762

L000766:                                                        ;000766
        movea.l (L001e28,pc),a0                                 ;000766
        movea.l (L001e24,pc),a1                                 ;00076a
        subq.w  #6,a0                                           ;00076e
        cmpa.l  a1,a0                                           ;000770
        bcc.s   L000788                                         ;000772
        movea.l (L001e2c,pc),a0                                 ;000774
        bra.s   L000788                                         ;000778

L00077a:                                                        ;00077a
        movea.l (L001e28,pc),a0                                 ;00077a
        addq.w  #6,a0                                           ;00077e
        tst.w   (a0)                                            ;000780
        bne.s   L000788                                         ;000782
        movea.l (L001e24,pc),a0                                 ;000784
L000788:                                                        ;000788
        move.l  a0,(-$03e4,a5)                                  ;000788
        move.w  (a0),(-$03ea,a5)                                ;00078c
        movea.l ($0002,a0),a1                                   ;000790
        move.l  (a1),($0010,a5)                                 ;000794
        addq.w  #4,a1                                           ;000798
        move.w  (a1),d0                                         ;00079a
        not.w   d0                                              ;00079c
        move.w  ($0002,a1),d1                                   ;00079e
        not.w   d1                                              ;0007a2
        move.b  d0,($0024,a5)                                   ;0007a4
        move.b  d1,($0025,a5)                                   ;0007a8
        move.l  a1,($000c,a5)                                   ;0007ac
        clr.w   (-$03f0,a5)                                     ;0007b0
        bra.s   StartPlay                                       ;0007b4

L_PLAY:                                                         ;0007b6
        clr.w   (-$03f0,a5)                                     ;0007b6
        bra.s   StartPlay                                       ;0007ba

L_PlayWithMask:                                                 ;0007bc
        move.w  d1,(-$03f0,a5)                                  ;0007bc
StartPlay:                                                      ;0007c0
        clr.b   (-$03f8,a5)                                     ;0007c0
        clr.b   (-$03f7,a5)                                     ;0007c4
        clr.b   (-$03f5,a5)                                     ;0007c8
        clr.b   (-$03f9,a5)                                     ;0007cc
        tst.b   (-$03fa,a5)                                     ;0007d0
        beq.s   L0007f4                                         ;0007d4
        movea.l ($0088),a0                                      ;0007d6
        move.l  (-$0008,a0),d0                                  ;0007da
        cmp.l   #$50434d34,d0           ;'PCM4'                 ;0007de
        beq.s   L0007ee                                         ;0007e4
        cmp.l   #$50434d38,d0           ;'PCM8'                 ;0007e6
        bne.s   L0007f4                                         ;0007ec
L0007ee:                                                        ;0007ee
        move.w  #$0100,d0                                       ;0007ee
        trap    #2                                              ;0007f2
L0007f4:                                                        ;0007f4
        clr.b   (-$03fa,a5)                                     ;0007f4
        clr.b   (-$0418,a5)                                     ;0007f8
        move.w  #$01ff,(-$03f2,a5)                              ;0007fc
        move.w  #$01ff,(-$0406,a5)                              ;000802
        clr.w   ($003a,a5)                                      ;000808
        clr.w   (-$0666,a5)                                     ;00080c
        move.b  ($0024,a5),d0                                   ;000810
        beq.w   L_ERROR                                         ;000814
        bsr.w   StopPlayback                                    ;000818
        movea.l ($000c,a5),a2                                   ;00081c
        move.w  ($0002,a2),d1                                   ;000820
        bmi.s   L000848                                         ;000824
        tst.b   ($0025,a5)                                      ;000826
        beq.w   L_ERROR                                         ;00082a
        movea.l ($0010,a5),a0                                   ;00082e
        bra.s   L00083c                                         ;000832

L000834:                                                        ;000834
        tst.l   (a0)                                            ;000834
        beq.w   L_ERROR                                         ;000836
        adda.l  (a0),a0                                         ;00083a
L00083c:                                                        ;00083c
        dbra.w  d1,L000834                                      ;00083c
        adda.w  ($0004,a0),a0                                   ;000840
        move.l  a0,($0020,a5)                                   ;000844
L000848:                                                        ;000848
        adda.w  ($0004,a2),a2                                   ;000848
        movea.l a2,a1                                           ;00084c
        movea.l a2,a0                                           ;00084e
        moveq.l #$00,d0                                         ;000850
        move.w  (a1)+,d0                                        ;000852
        adda.l  d0,a2                                           ;000854
        move.l  a2,($001c,a5)                                   ;000856
        lea.l   (L001f3c,pc),a6                                 ;00085a
        lea.l   (L00095a,pc),a3                                 ;00085e
        moveq.l #$ff,d6                                         ;000862
        moveq.l #$00,d7                                         ;000864
L000866:                                                        ;000866
        movea.l a0,a2                                           ;000866
        move.w  (a1)+,d0                                        ;000868
        adda.l  d0,a2                                           ;00086a
        move.l  a2,(a6)                                         ;00086c
        move.l  a3,($0026,a6)                                   ;00086e
        move.l  a3,($0040,a6)                                   ;000872
        move.w  d6,($0014,a6)                                   ;000876
        move.b  d6,($0023,a6)                                   ;00087a
        move.b  d7,($0018,a6)                                   ;00087e
        move.b  #$00,($001d,a6)                                 ;000882
        move.b  #$01,($001a,a6)                                 ;000888
        move.b  #$08,($0022,a6)                                 ;00088e
        move.b  #$c0,($001c,a6)                                 ;000894
        move.b  #$08,($001e,a6)                                 ;00089a
        clr.w   ($0036,a6)                                      ;0008a0
        clr.w   ($004a,a6)                                      ;0008a4
        clr.w   ($0010,a6)                                      ;0008a8
        clr.b   ($0024,a6)                                      ;0008ac
        clr.b   ($001f,a6)                                      ;0008b0
        clr.b   ($0019,a6)                                      ;0008b4
        clr.w   ($0016,a6)                                      ;0008b8
        cmp.w   #$0008,d7                                       ;0008bc
        bcc.s   L0008d4                                         ;0008c0
        moveq.l #$38,d1                 ;'8'                    ;0008c2
        add.b   d7,d1                                           ;0008c4
        moveq.l #$00,d2                                         ;0008c6
        bsr.w   L_WRITEOPM                                      ;0008c8
        addq.w  #1,d7                                           ;0008cc
        lea.l   ($0050,a6),a6                                   ;0008ce
        bra.s   L000866                                         ;0008d2

L0008d4:                                                        ;0008d4
        move.b  #$10,($001c,a6)                                 ;0008d4
        move.b  #$08,($0022,a6)                                 ;0008da
        move.b  d7,($0018,a6)                                   ;0008e0
        andi.b  #$07,($0018,a6)                                 ;0008e4
        ori.b   #$80,($0018,a6)                                 ;0008ea
        clr.b   ($0004,a6)                                      ;0008f0
        cmp.w   #$000f,d7                                       ;0008f4
        beq.s   L000910                                         ;0008f8
        addq.w  #1,d7                                           ;0008fa
        lea.l   ($0050,a6),a6                                   ;0008fc
        cmp.w   #$0009,d7                                       ;000900
        bne.w   L000866                                         ;000904
        lea.l   (L001bc4,pc),a6                                 ;000908
        bra.w   L000866                                         ;00090c

L000910:                                                        ;000910
        lea.l   (-$0416,a5),a0                                  ;000910
        moveq.l #$0f,d0                                         ;000914
L000916:                                                        ;000916
        clr.b   (a0)+                                           ;000916
        dbra.w  d0,L000916                                      ;000918
        clr.b   ($0026,a5)                                      ;00091c
        moveq.l #$00,d2                                         ;000920
        moveq.l #$01,d1                                         ;000922
        bsr.w   L_WRITEOPM                                      ;000924
        moveq.l #$0f,d1                                         ;000928
        bsr.w   L_WRITEOPM                                      ;00092a
        moveq.l #$19,d1                                         ;00092e
        bsr.w   L_WRITEOPM                                      ;000930
        moveq.l #$80,d2                                         ;000934
        bsr.w   L_WRITEOPM                                      ;000936
        moveq.l #$c8,d2                                         ;00093a
        moveq.l #$12,d1                                         ;00093c
        move.b  d2,(-$0400,a5)                                  ;00093e
        tst.b   (-$0404,a5)                                     ;000942
        bne.s   L00094c                                         ;000946
        bsr.w   L_WRITEOPM                                      ;000948
L00094c:                                                        ;00094c
        bsr.w   L00056a                                         ;00094c
        bsr.w   EnableTimerIRQ                                  ;000950
        moveq.l #$00,d0                                         ;000954
        rts                                                     ;000956

L_ERROR:                                                        ;000958
        moveq.l #$ff,d0                                         ;000958
L00095a:                                                        ;00095a
        rts                                                     ;00095a

L_GetMDXTitle:                                                  ;00095c
        tst.b   ($0024,a5)                                      ;00095c
        beq.s   L000998                                         ;000960
        movea.l ($000c,a5),a0                                   ;000962
        bra.s   L00096e                                         ;000966

L000968:                                                        ;000968
        tst.w   (a0)                                            ;000968
        beq.s   L000998                                         ;00096a
        adda.w  (a0),a0                                         ;00096c
L00096e:                                                        ;00096e
        dbra.w  d1,L000968                                      ;00096e
        adda.w  ($0006,a0),a0                                   ;000972
        move.l  a0,d0                                           ;000976
        rts                                                     ;000978

L_GetPDXFilename:                                               ;00097a
        tst.b   ($0025,a5)                                      ;00097a
        beq.s   L000998                                         ;00097e
        movea.l ($0010,a5),a0                                   ;000980
        bra.s   L00098c                                         ;000984

L000986:                                                        ;000986
        tst.l   (a0)                                            ;000986
        beq.s   L000998                                         ;000988
        adda.l  (a0),a0                                         ;00098a
L00098c:                                                        ;00098c
        dbra.w  d1,L000986                                      ;00098c
        adda.w  ($0006,a0),a0                                   ;000990
        move.l  a0,d0                                           ;000994
        rts                                                     ;000996

L000998:                                                        ;000998
        moveq.l #$00,d0                                         ;000998
        rts                                                     ;00099a

L_OPMINT:                                                       ;00099c
        andi.b  #$f7,($00e88015)                                ;00099c
        move.l  a6,-(sp)                                        ;0009a4
        movea.l ($0006,sp),a6                                   ;0009a6
        cmpa.l  ($01a8),a6                                      ;0009aa
        bne.s   L0009be                                         ;0009ae
        pea.l   (L0009b8,pc)                                    ;0009b0
        move.w  sr,-(sp)                                        ;0009b4
        jmp     (a6)                                            ;0009b6

L0009b8:                                                        ;0009b8
        movea.l (sp)+,a6                                        ;0009b8
        addq.w  #6,sp                                           ;0009ba
        move.l  a6,-(sp)                                        ;0009bc
L0009be:                                                        ;0009be
        andi.w  #$faff,sr                                       ;0009be
        movem.l d0-d7/a0-a5,-(sp)                               ;0009c2
        lea.l   (L00220c,pc),a5                                 ;0009c6
        st.b    ($0039,a5)                                      ;0009ca
        tst.b   (-$03fa,a5)                                     ;0009ce
        bne.w   L000a66                                         ;0009d2
        lea.l   (L001e1e,pc),a0                                 ;0009d6
        lea.l   (fadeout_enable,pc),a1                          ;0009da
        tst.b   (a1)                                            ;0009de
        beq.w   L000a66                                         ;0009e0
        bpl.s   L0009ee                                         ;0009e4
        move.b  #$7f,(a1)                                       ;0009e6
        move.w  (a0),($0002,a0)                                 ;0009ea
L0009ee:                                                        ;0009ee
        tst.w   ($0002,a0)                                      ;0009ee
        bmi.s   L0009fa                                         ;0009f2
        subq.w  #2,($0002,a0)                                   ;0009f4
        bra.s   L000a66                                         ;0009f8

L0009fa:                                                        ;0009fa
        lea.l   (fadeout_offset,pc),a1                          ;0009fa
        cmpi.b  #$0a,(a1)                                       ;0009fe
        bge.s   L000a12                                         ;000a02
L000a04:                                                        ;000a04
        cmpi.b  #$3e,(a1)               ;'>'                    ;000a04
        bge.s   L000a18                                         ;000a08
        addq.b  #1,(a1)                                         ;000a0a
        move.w  (a0),($0002,a0)                                 ;000a0c
        bra.s   L000a66                                         ;000a10

L000a12:                                                        ;000a12
        st.b    (-$03f7,a5)                                     ;000a12
        bra.s   L000a04                                         ;000a16

L000a18:                                                        ;000a18
        tst.b   (-$03f4,a5)                                     ;000a18
        beq.s   L000a26                                         ;000a1c
        bsr.w   L00077a                                         ;000a1e
        bra.w   L000c40                                         ;000a22

L000a26:                                                        ;000a26
        move.b  #$7f,(a1)                                       ;000a26
        clr.b   (-$03f5,a5)                                     ;000a2a
        move.b  #$01,(-$03f9,a5)                                ;000a2e
        bsr.w   L_PAUSE                                         ;000a34
        movea.l ($0088),a0                                      ;000a38
        move.l  (-$0008,a0),d0                                  ;000a3c
        cmp.l   #$50434d34,d0           ;'PCM4'                 ;000a40
        beq.s   L000a50                                         ;000a46
        cmp.l   #$50434d38,d0           ;'PCM8'                 ;000a48
        bne.s   L000a56                                         ;000a4e
L000a50:                                                        ;000a50
        move.w  #$0100,d0                                       ;000a50
        trap    #2                                              ;000a54
L000a56:                                                        ;000a56
        tst.b   (-$0418,a5)                                     ;000a56
        beq.s   L000a66                                         ;000a5a
        move.w  #$01ff,d0                                       ;000a5c
        trap    #2                                              ;000a60
        clr.b   (-$0418,a5)                                     ;000a62
L000a66:                                                        ;000a66
        lea.l   (opm_tempo,pc),a0                               ;000a66
        move.b  (a0),d2                                         ;000a6a
        moveq.l #$12,d1                                         ;000a6c
        tst.b   (-$0402,a5)                                     ;000a6e
        bne.w   L000bd0                                         ;000a72
        move.b  ($080e),d7                                      ;000a76
        and.b   #$0f,d7                                         ;000a7a
        cmp.b   #$05,d7                                         ;000a7e
        beq.w   L000b16                                         ;000a82
        cmp.b   #$09,d7                                         ;000a86
        beq.s   L000b0a                                         ;000a8a
        cmp.b   #$06,d7                                         ;000a8c
        beq.s   L000b04                                         ;000a90
        cmp.b   #$0a,d7                                         ;000a92
        beq.s   L000afc                                         ;000a96
        cmp.b   #$04,d7                                         ;000a98
        beq.w   L000b6c                                         ;000a9c
        cmp.b   #$08,d7                                         ;000aa0
        beq.s   L000b20                                         ;000aa4
        cmp.b   #$02,d7                                         ;000aa6
        bne.w   L000bd0                                         ;000aaa
        move.b  ($080b),d7                                      ;000aae
        and.b   #$03,d7                                         ;000ab2
        cmpi.b  #$80,($080a)                                    ;000ab6
        beq.s   L000aec                                         ;000abc
        cmp.b   #$02,d7                                         ;000abe
        beq.s   L000adc                                         ;000ac2
        cmp.b   #$01,d7                                         ;000ac4
        bne.w   L000bd0                                         ;000ac8
        tst.b   (-$03fc,a5)                                     ;000acc
        bne.w   L000bca                                         ;000ad0
        subq.b  #1,($0002,a0)                                   ;000ad4
        bra.w   L000bca                                         ;000ad8

L000adc:                                                        ;000adc
        tst.b   (-$03fc,a5)                                     ;000adc
        bne.w   L000bca                                         ;000ae0
        addq.b  #1,($0002,a0)                                   ;000ae4
        bra.w   L000bca                                         ;000ae8

L000aec:                                                        ;000aec
        tst.b   (-$03fc,a5)                                     ;000aec
        bne.w   L000bca                                         ;000af0
        clr.b   ($0002,a0)                                      ;000af4
        bra.w   L000bca                                         ;000af8

L000afc:                                                        ;000afc
        move.b  #$ff,d2                                         ;000afc
        bra.w   L000bd8                                         ;000b00

L000b04:                                                        ;000b04
        moveq.l #$00,d2                                         ;000b04
        bra.w   L000bd8                                         ;000b06

L000b0a:                                                        ;000b0a
        neg.b   d2                                              ;000b0a
        lsr.b   #2,d2                                           ;000b0c
        addq.b  #1,d2                                           ;000b0e
        neg.b   d2                                              ;000b10
        bra.w   L000bd8                                         ;000b12

L000b16:                                                        ;000b16
        neg.b   d2                                              ;000b16
        add.b   d2,d2                                           ;000b18
        neg.b   d2                                              ;000b1a
        bra.w   L000bd8                                         ;000b1c

L000b20:                                                        ;000b20
        btst.b  #$00,($080b)                                    ;000b20
        bne.s   L000b56                                         ;000b26
        btst.b  #$01,($080b)                                    ;000b28
        beq.w   L000bd0                                         ;000b2e
        tst.b   (-$03fa,a5)                                     ;000b32
        beq.s   L000b4a                                         ;000b36
        bpl.w   L000bbe                                         ;000b38
        tst.b   (-$03fc,a5)                                     ;000b3c
        bne.w   L000bca                                         ;000b40
        bsr.w   L_CONT                                          ;000b44
        bra.s   L000bbe                                         ;000b48

L000b4a:                                                        ;000b4a
        tst.b   (-$03fc,a5)                                     ;000b4a
        bne.s   L000bca                                         ;000b4e
        bsr.w   L_PAUSE                                         ;000b50
        bra.s   L000bbe                                         ;000b54

L000b56:                                                        ;000b56
        tst.b   (-$03fa,a5)                                     ;000b56
        beq.s   L000bca                                         ;000b5a
        tst.b   (-$03fc,a5)                                     ;000b5c
        bne.s   L000bca                                         ;000b60
        st.b    (-$03fc,a5)                                     ;000b62
        moveq.l #$00,d2                                         ;000b66
        bra.w   L000bec                                         ;000b68

L000b6c:                                                        ;000b6c
        move.b  ($080b),d7                                      ;000b6c
        beq.s   L000b82                                         ;000b70
        and.b   #$03,d7                                         ;000b72
        cmp.b   #$02,d7                                         ;000b76
        beq.s   L000bae                                         ;000b7a
        cmp.b   #$01,d7                                         ;000b7c
        beq.s   L000b9c                                         ;000b80
L000b82:                                                        ;000b82
        cmpi.b  #$80,($080a)                                    ;000b82
        bne.s   L000bd0                                         ;000b88
        tst.b   (-$03fc,a5)                                     ;000b8a
        bne.s   L000bca                                         ;000b8e
L000b90:                                                        ;000b90
        move.w  #$0011,(-$03ee,a5)                              ;000b90
        st.b    (-$03f5,a5)                                     ;000b96
        bra.s   L000bca                                         ;000b9a

L000b9c:                                                        ;000b9c
        tst.b   (-$03fc,a5)                                     ;000b9c
        bne.s   L000bca                                         ;000ba0
        tst.b   (-$03f4,a5)                                     ;000ba2
        beq.s   L000bc4                                         ;000ba6
        bsr.w   L000766                                         ;000ba8
        bra.s   L000bbe                                         ;000bac

L000bae:                                                        ;000bae
        tst.b   (-$03fc,a5)                                     ;000bae
        bne.s   L000bca                                         ;000bb2
        tst.b   (-$03f4,a5)                                     ;000bb4
        beq.s   L000bc4                                         ;000bb8
        bsr.w   L00077a                                         ;000bba
L000bbe:                                                        ;000bbe
        st.b    (-$03fc,a5)                                     ;000bbe
        bra.s   L000c40                                         ;000bc2

L000bc4:                                                        ;000bc4
        bsr.w   L_PLAY                                          ;000bc4
        bra.s   L000bbe                                         ;000bc8

L000bca:                                                        ;000bca
        st.b    (-$03fc,a5)                                     ;000bca
        bra.s   L000bd4                                         ;000bce

L000bd0:                                                        ;000bd0
        clr.b   (-$03fc,a5)                                     ;000bd0
L000bd4:                                                        ;000bd4
        add.b   ($0002,a0),d2                                   ;000bd4
L000bd8:                                                        ;000bd8
        tst.b   (-$03fa,a5)                                     ;000bd8
        bne.s   L000be4                                         ;000bdc
        tst.b   (-$03f9,a5)                                     ;000bde
        beq.s   L000bec                                         ;000be2
L000be4:                                                        ;000be4
        moveq.l #$00,d2                                         ;000be4
        bsr.w   L_WRITEOPM                                      ;000be6
        bra.s   L000c40                                         ;000bea

L000bec:                                                        ;000bec
        bsr.w   L_WRITEOPM                                      ;000bec
        addq.w  #1,(-$0666,a5)                                  ;000bf0
        lea.l   (L001f3c,pc),a6                                 ;000bf4
        moveq.l #$00,d7                                         ;000bf8
L000bfa:                                                        ;000bfa
        bsr.w   L001050                                         ;000bfa
        bsr.w   L0011b4                                         ;000bfe
        move.w  (channel_mask,pc),d0                            ;000c02
        btst.l  d7,d0                                           ;000c06
        bne.s   L000c0c                                         ;000c08
        bsr.s   L000c66                                         ;000c0a
L000c0c:                                                        ;000c0c
        lea.l   ($0050,a6),a6                                   ;000c0c
        addq.w  #1,d7                                           ;000c10
        cmp.w   #$0009,d7                                       ;000c12
        bcs.s   L000bfa                                         ;000c16
        tst.b   (-$0418,a5)                                     ;000c18
        beq.s   L000c40                                         ;000c1c
        lea.l   (L001bc4,pc),a6                                 ;000c1e
L000c22:                                                        ;000c22
        bsr.w   L001050                                         ;000c22
        bsr.w   L0011b4                                         ;000c26
        move.w  (channel_mask,pc),d0                            ;000c2a
        btst.l  d7,d0                                           ;000c2e
        bne.s   L000c34                                         ;000c30
        bsr.s   L000c66                                         ;000c32
L000c34:                                                        ;000c34
        lea.l   ($0050,a6),a6                                   ;000c34
        addq.w  #1,d7                                           ;000c38
        cmp.w   #$0010,d7                                       ;000c3a
        bcs.s   L000c22                                         ;000c3e
L000c40:                                                        ;000c40
        bsr.w   EnableTimerIRQ                                  ;000c40
L000c44:                                                        ;000c44
        tst.b   ($00e90003)                                     ;000c44
        bmi.s   L000c44                                         ;000c4a
        move.b  #$1b,($00e90001)                                ;000c4c
        clr.b   ($0039,a5)                                      ;000c54
        movem.l (sp)+,d0-d7/a0-a6                               ;000c58
        ori.b   #$08,($00e88015)                                ;000c5c
        rte                                                     ;000c64

L000c66:                                                        ;000c66
        btst.b  #$00,($0016,a6)                                 ;000c66
        beq.s   L000cce                                         ;000c6c
        tst.b   ($0020,a6)                                      ;000c6e
        bne.s   L000cca                                         ;000c72
        tst.b   ($0018,a6)                                      ;000c74
        bmi.s   L000cbe                                         ;000c78
        bsr.w   LoadVoice                                       ;000c7a
        bsr.w   WritePan                                        ;000c7e
        btst.b  #$03,($0016,a6)                                 ;000c82
        bne.s   L000cb4                                         ;000c88
        move.b  ($0024,a6),($0025,a6)                           ;000c8a
        beq.s   L000c9e                                         ;000c90
        clr.l   ($0036,a6)                                      ;000c92
        clr.w   ($004a,a6)                                      ;000c96
        bsr.w   AdvanceLFODelay                                 ;000c9a
L000c9e:                                                        ;000c9e
        btst.b  #$01,($0016,a6)                                 ;000c9e
        beq.s   L000cb4                                         ;000ca4
        moveq.l #$01,d1                                         ;000ca6
        moveq.l #$02,d2                                         ;000ca8
        bsr.w   L_WRITEOPM                                      ;000caa
        moveq.l #$00,d2                                         ;000cae
        bsr.w   L_WRITEOPM                                      ;000cb0
L000cb4:                                                        ;000cb4
        clr.l   ($000c,a6)                                      ;000cb4
        bsr.s   SetOPMPitch                                     ;000cb8
        bsr.w   SetOPMVolume                                    ;000cba
L000cbe:                                                        ;000cbe
        bsr.w   SetPCMVolume                                    ;000cbe
        andi.b  #$fe,($0016,a6)                                 ;000cc2
        rts                                                     ;000cc8

L000cca:                                                        ;000cca
        subq.b  #1,($0020,a6)                                   ;000cca
L000cce:                                                        ;000cce
        tst.b   ($0018,a6)                                      ;000cce
        bmi.s   L000cda                                         ;000cd2
        bsr.s   SetOPMPitch                                     ;000cd4
        bsr.w   SetOPMVolume                                    ;000cd6
L000cda:                                                        ;000cda
        rts                                                     ;000cda

SetOPMPitch:                                                    ;000cdc
        move.w  ($0012,a6),d2                                   ;000cdc
        add.w   ($000c,a6),d2                                   ;000ce0
        add.w   ($0036,a6),d2                                   ;000ce4
        cmp.w   ($0014,a6),d2                                   ;000ce8
        beq.s   L000d22                                         ;000cec
        move.w  d2,($0014,a6)                                   ;000cee
        move.w  #$17ff,d1                                       ;000cf2
        cmp.w   d1,d2                                           ;000cf6
        bls.s   L000d04                                         ;000cf8
        tst.w   d2                                              ;000cfa
        bpl.s   L000d02                                         ;000cfc
        moveq.l #$00,d2                                         ;000cfe
        bra.s   L000d04                                         ;000d00

L000d02:                                                        ;000d02
        move.w  d1,d2                                           ;000d02
L000d04:                                                        ;000d04
        add.w   d2,d2                                           ;000d04
        add.w   d2,d2                                           ;000d06
        moveq.l #$30,d1                 ;'0'                    ;000d08
        add.b   ($0018,a6),d1                                   ;000d0a
        bsr.w   L_WRITEOPM                                      ;000d0e
        subq.b  #8,d1                                           ;000d12
        move.w  d2,-(sp)                                        ;000d14
        moveq.l #$00,d2                                         ;000d16
        move.b  (sp)+,d2                                        ;000d18
        move.b  (OPMNoteTable,pc,d2.w),d2                       ;000d1a
        bsr.w   L_WRITEOPM                                      ;000d1e
L000d22:                                                        ;000d22
        rts                                                     ;000d22

OPMNoteTable:                                                   ;000d24
        .dc.b   $00,$01,$02,$04,$05,$06,$08,$09                 ;000d24
        .dc.b   $0a,$0c,$0d,$0e,$10,$11,$12,$14                 ;000d2c
        .dc.b   $15,$16,$18,$19,$1a,$1c,$1d,$1e                 ;000d34
        .dc.b   $20,$21,$22,$24,$25,$26,$28,$29                 ;000d3c
        .dc.b   $2a,$2c,$2d,$2e,$30,$31,$32,$34                 ;000d44
        .dc.b   $35,$36,$38,$39,$3a,$3c,$3d,$3e                 ;000d4c
        .dc.b   $40,$41,$42,$44,$45,$46,$48,$49                 ;000d54
        .dc.b   $4a,$4c,$4d,$4e,$50,$51,$52,$54                 ;000d5c
        .dc.b   $55,$56,$58,$59,$5a,$5c,$5d,$5e                 ;000d64
        .dc.b   $60,$61,$62,$64,$65,$66,$68,$69                 ;000d6c
        .dc.b   $6a,$6c,$6d,$6e,$70,$71,$72,$74                 ;000d74
        .dc.b   $75,$76,$78,$79,$7a,$7c,$7d,$7e                 ;000d7c
LoadVoice:                                                      ;000d84
        bclr.b  #$01,($0017,a6)                                 ;000d84
        beq.s   L000df4                                         ;000d8a
        movea.l ($0004,a6),a0                                   ;000d8c
        andi.b  #$c0,($001c,a6)                                 ;000d90
        move.b  (a0)+,d0                                        ;000d96
        or.b    d0,($001c,a6)                                   ;000d98
        and.w   #$0007,d0                                       ;000d9c
        move.b  (CarrierSlot,pc,d0.w),d3                        ;000da0
        move.b  d3,($0019,a6)                                   ;000da4
        move.b  (a0)+,d0                                        ;000da8
        lsl.b   #3,d0                                           ;000daa
        or.b    ($0018,a6),d0                                   ;000dac
        move.b  d0,($001d,a6)                                   ;000db0
        moveq.l #$40,d1                 ;'@'                    ;000db4
        add.b   ($0018,a6),d1                                   ;000db6
        moveq.l #$03,d0                                         ;000dba
L000dbc:                                                        ;000dbc
        move.b  (a0)+,d2                                        ;000dbc
        bsr.w   L_WRITEOPM                                      ;000dbe
        addq.b  #8,d1                                           ;000dc2
        dbra.w  d0,L000dbc                                      ;000dc4
        moveq.l #$03,d0                                         ;000dc8
L000dca:                                                        ;000dca
        move.b  (a0)+,d2                                        ;000dca
        lsr.b   #1,d3                                           ;000dcc
        bcc.s   L000dd2                                         ;000dce
        moveq.l #$7f,d2                                         ;000dd0
L000dd2:                                                        ;000dd2
        bsr.w   L_WRITEOPM                                      ;000dd2
        addq.b  #8,d1                                           ;000dd6
        dbra.w  d0,L000dca                                      ;000dd8
        moveq.l #$0f,d0                                         ;000ddc
L000dde:                                                        ;000dde
        move.b  (a0)+,d2                                        ;000dde
        bsr.w   L_WRITEOPM                                      ;000de0
        addq.b  #8,d1                                           ;000de4
        dbra.w  d0,L000dde                                      ;000de6
        st.b    ($0023,a6)                                      ;000dea
        ori.b   #$64,($0017,a6)                                 ;000dee
L000df4:                                                        ;000df4
        rts                                                     ;000df4

CarrierSlot:                                                    ;000df6
        .dc.b   $08,$08,$08,$08,$0c,$0e,$0e,$0f                 ;000df6
SetOPMVolume:                                                   ;000dfe
        moveq.l #$00,d0                                         ;000dfe
        move.b  ($0022,a6),d0                                   ;000e00
        bclr.l  #$07,d0                                         ;000e04
        bne.s   L000e0e                                         ;000e08
        move.b  (VolumeTable,pc,d0.w),d0                        ;000e0a
L000e0e:                                                        ;000e0e
        add.b   (fadeout_offset,pc),d0                          ;000e0e
        bcs.s   L000e16                                         ;000e12
        bpl.s   L000e18                                         ;000e14
L000e16:                                                        ;000e16
        moveq.l #$7f,d0                                         ;000e16
L000e18:                                                        ;000e18
        add.b   ($004a,a6),d0                                   ;000e18
        bcs.s   L000e20                                         ;000e1c
        bpl.s   L000e22                                         ;000e1e
L000e20:                                                        ;000e20
        moveq.l #$7f,d0                                         ;000e20
L000e22:                                                        ;000e22
        cmp.b   ($0023,a6),d0                                   ;000e22
        beq.s   L000e54                                         ;000e26
WriteTL:                                                        ;000e28
        move.b  d0,($0023,a6)                                   ;000e28
        movea.l ($0004,a6),a0                                   ;000e2c
        addq.w  #6,a0                                           ;000e30
        move.b  ($0019,a6),d3                                   ;000e32
        moveq.l #$60,d1                 ;'`'                    ;000e36
        add.b   ($0018,a6),d1                                   ;000e38
        moveq.l #$03,d4                                         ;000e3c
L000e3e:                                                        ;000e3e
        move.b  (a0)+,d2                                        ;000e3e
        lsr.b   #1,d3                                           ;000e40
        bcc.s   L000e4e                                         ;000e42
        add.b   d0,d2                                           ;000e44
        bpl.s   L000e4a                                         ;000e46
        moveq.l #$7f,d2                                         ;000e48
L000e4a:                                                        ;000e4a
        bsr.w   L_WRITEOPM                                      ;000e4a
L000e4e:                                                        ;000e4e
        addq.b  #8,d1                                           ;000e4e
        dbra.w  d4,L000e3e                                      ;000e50
L000e54:                                                        ;000e54
        rts                                                     ;000e54

VolumeTable:                                                    ;000e56
        .dc.b   $2a,$28,$25,$22,$20,$1d,$1a,$18                 ;000e56
        .dc.b   $15,$12,$10,$0d,$0a,$08,$05,$02                 ;000e5e
WritePan:                                                       ;000e66
        bclr.b  #$02,($0017,a6)                                 ;000e66
        beq.s   L000e7c                                         ;000e6c
        move.b  ($001c,a6),d2                                   ;000e6e
        moveq.l #$20,d1                 ;' '                    ;000e72
        add.b   ($0018,a6),d1                                   ;000e74
        bra.w   L_WRITEOPM                                      ;000e78

L000e7c:                                                        ;000e7c
        rts                                                     ;000e7c

SetPCMVolume:                                                   ;000e7e
        bset.b  #$03,($0016,a6)                                 ;000e7e
        bne.s   L000e7c                                         ;000e84
        btst.b  #$04,($0016,a6)                                 ;000e86
        beq.s   L000e92                                         ;000e8c
        bsr.w   SendKeyOff                                      ;000e8e
L000e92:                                                        ;000e92
        tst.b   ($0018,a6)                                      ;000e92
        bmi.s   L000eb2                                         ;000e96
        move.b  ($001d,a6),d2                                   ;000e98
        lea.l   (L00223c,pc),a2                                 ;000e9c
        move.b  d2,(a2,d7.w)                                    ;000ea0
        lea.l   (pcm_work_area,pc),a2                           ;000ea4
        move.b  d2,(a2,d7.w)                                    ;000ea8
        moveq.l #$08,d1                                         ;000eac
        bra.w   L_WRITEOPM                                      ;000eae

L000eb2:                                                        ;000eb2
        move.b  ($0025,a5),d0                                   ;000eb2
        beq.s   L000f26                                         ;000eb6
        tst.b   (-$0403,a5)                                     ;000eb8
        bne.s   L000f26                                         ;000ebc
        moveq.l #$00,d0                                         ;000ebe
        move.w  ($0012,a6),d0                                   ;000ec0
        lsr.w   #6,d0                                           ;000ec4
        move.b  ($001c,a6),d2                                   ;000ec6
        move.b  d2,d1                                           ;000eca
        and.w   #$0003,d1                                       ;000ecc
        beq.s   L000ed8                                         ;000ed0
        cmp.w   #$0003,d1                                       ;000ed2
        bne.s   L000edc                                         ;000ed6
L000ed8:                                                        ;000ed8
        eori.w  #$0003,d1                                       ;000ed8
L000edc:                                                        ;000edc
        and.w   #$001c,d2                                       ;000edc
        lsl.w   #6,d2                                           ;000ee0
        or.w    d1,d2                                           ;000ee2
        tst.b   (-$0418,a5)                                     ;000ee4
        bne.s   L000f28                                         ;000ee8
        tst.b   (-$03f7,a5)                                     ;000eea
        beq.s   L000ef4                                         ;000eee
        andi.b  #$fc,d2                                         ;000ef0
L000ef4:                                                        ;000ef4
        lsl.w   #3,d0                                           ;000ef4
        movea.l ($0020,a5),a1                                   ;000ef6
        lea.l   (a1,d0.w),a0                                    ;000efa
        adda.l  (a0)+,a1                                        ;000efe
        addq.w  #2,a0                                           ;000f00
        move.w  (a0)+,d3                                        ;000f02
        beq.s   L000f26                                         ;000f04
        moveq.l #$67,d0                 ;'g'                    ;000f06
        moveq.l #$00,d1                                         ;000f08
        trap    #15                                             ;000f0a
        moveq.l #$60,d0                 ;'`'                    ;000f0c
        move.w  d2,d1                                           ;000f0e
        moveq.l #$00,d2                                         ;000f10
        move.w  d3,d2                                           ;000f12
        trap    #15                                             ;000f14
        lea.l   (L00223c,pc),a2                                 ;000f16
        clr.b   (a2,d7.w)                                       ;000f1a
        lea.l   (pcm_work_area,pc),a2                           ;000f1e
        clr.b   (a2,d7.w)                                       ;000f22
L000f26:                                                        ;000f26
        rts                                                     ;000f26

L000f28:                                                        ;000f28
        moveq.l #$00,d1                                         ;000f28
        move.b  ($0004,a6),d1                                   ;000f2a
        lsl.l   #5,d1                                           ;000f2e
        add.l   d1,d0                                           ;000f30
        add.l   d1,d1                                           ;000f32
        add.l   d1,d0                                           ;000f34
        lsl.l   #3,d0                                           ;000f36
        movea.l ($0020,a5),a1                                   ;000f38
        lea.l   (a1,d0.l),a0                                    ;000f3c
        move.l  ($0004,a0),d3                                   ;000f40
        beq.s   L000f26                                         ;000f44
        adda.l  (a0),a1                                         ;000f46
        lea.l   (L002248,pc),a0                                 ;000f48
        cmpa.l  a0,a1                                           ;000f4c
        bcs.s   L000fb8                                         ;000f4e
        move.l  a1,d0                                           ;000f50
        add.l   d3,d0                                           ;000f52
        bcs.s   L000fb8                                         ;000f54
        cmp.l   (-$065c,a5),d0                                  ;000f56
        bcc.s   L000fb8                                         ;000f5a
        move.b  ($0018,a6),d0                                   ;000f5c
        and.w   #$0007,d0                                       ;000f60
        moveq.l #$00,d1                                         ;000f64
        move.b  ($0022,a6),d1                                   ;000f66
        bclr.l  #$07,d1                                         ;000f6a
        bne.s   L000f78                                         ;000f6e
        lea.l   (VolumeTable,pc),a2                             ;000f70
        move.b  (a2,d1.w),d1                                    ;000f74
L000f78:                                                        ;000f78
        add.b   (fadeout_offset,pc),d1                          ;000f78
        bmi.s   L000f84                                         ;000f7c
        cmp.b   #$2b,d1                 ;'+'                    ;000f7e
        bcs.s   L000f8a                                         ;000f82
L000f84:                                                        ;000f84
        moveq.l #$00,d1                                         ;000f84
        clr.b   d2                                              ;000f86
        bra.s   L000f8e                                         ;000f88

L000f8a:                                                        ;000f8a
        move.b  (PCMVolumeTable,pc,d1.w),d1                     ;000f8a
L000f8e:                                                        ;000f8e
        swap.w  d1                                              ;000f8e
        move.w  d2,d1                                           ;000f90
        moveq.l #$00,d2                                         ;000f92
        trap    #2                                              ;000f94
        move.b  ($0018,a6),d0                                   ;000f96
        and.w   #$0007,d0                                       ;000f9a
        move.l  d3,d2                                           ;000f9e
        andi.l  #$00ffffff,d2                                   ;000fa0
        trap    #2                                              ;000fa6
        lea.l   (L00223c,pc),a2                                 ;000fa8
        clr.b   ($0008,a2)                                      ;000fac
        lea.l   (pcm_work_area,pc),a2                           ;000fb0
        clr.b   (a2,d7.w)                                       ;000fb4
L000fb8:                                                        ;000fb8
        rts                                                     ;000fb8

PCMVolumeTable:                                                 ;000fba
        .dc.b   $0f,$0f,$0f,$0e,$0e,$0e,$0d,$0d                 ;000fba
        .dc.b   $0d,$0c,$0c,$0b,$0b,$0b,$0a,$0a                 ;000fc2
        .dc.b   $0a,$09,$09,$08,$08,$08,$07,$07                 ;000fca
        .dc.b   $07,$06,$06,$05,$05,$05,$04,$04                 ;000fd2
        .dc.b   $04,$03,$03,$02,$02,$02,$01,$01                 ;000fda
        .dc.b   $01,$00,$00,$ff                                 ;000fe2
L000fe6:                                                        ;000fe6
        bclr.b  #$03,($0016,a6)                                 ;000fe6
        beq.s   L00103a                                         ;000fec
        btst.b  #$04,($0016,a6)                                 ;000fee
        bne.s   L00103a                                         ;000ff4
SendKeyOff:                                                     ;000ff6
        move.b  ($0018,a6),d2                                   ;000ff6
        bmi.s   L001012                                         ;000ffa
        moveq.l #$08,d1                                         ;000ffc
        lea.l   (L00223c,pc),a2                                 ;000ffe
        move.b  d2,(a2,d7.w)                                    ;001002
        lea.l   (pcm_work_area,pc),a2                           ;001006
        move.b  d2,(a2,d7.w)                                    ;00100a
        bra.w   L_WRITEOPM                                      ;00100e

L001012:                                                        ;001012
        move.b  ($0025,a5),d0                                   ;001012
        beq.s   L00103a                                         ;001016
        tst.b   (-$0403,a5)                                     ;001018
        bne.s   L00103a                                         ;00101c
        tst.b   (-$0418,a5)                                     ;00101e
        beq.s   L00103c                                         ;001022
        move.b  ($0018,a6),d0                                   ;001024
        and.w   #$0007,d0                                       ;001028
        moveq.l #$00,d1                                         ;00102c
        move.b  ($0022,a6),d1                                   ;00102e
        swap.w  d1                                              ;001032
        move.w  d2,d1                                           ;001034
        moveq.l #$00,d2                                         ;001036
        trap    #2                                              ;001038
L00103a:                                                        ;00103a
        rts                                                     ;00103a

L00103c:                                                        ;00103c
        tst.b   ($0017,a6)                                      ;00103c
        bne.s   L001048                                         ;001040
        moveq.l #$67,d0                 ;'g'                    ;001042
        moveq.l #$01,d1                                         ;001044
        trap    #15                                             ;001046
L001048:                                                        ;001048
        moveq.l #$67,d0                 ;'g'                    ;001048
        moveq.l #$00,d1                                         ;00104a
        trap    #15                                             ;00104c
        rts                                                     ;00104e

L001050:                                                        ;001050
        tst.b   ($0018,a6)                                      ;001050
        bmi.s   L001092                                         ;001054
        tst.b   ($0016,a6)                                      ;001056
        bpl.s   L00106a                                         ;00105a
        tst.b   ($0020,a6)                                      ;00105c
        bne.s   L00106a                                         ;001060
        move.l  ($0008,a6),d0                                   ;001062
        add.l   d0,($000c,a6)                                   ;001066
L00106a:                                                        ;00106a
        tst.b   ($0024,a6)                                      ;00106a
        beq.s   L00107c                                         ;00106e
        tst.b   ($0020,a6)                                      ;001070
        bne.s   L001092                                         ;001074
        tst.b   ($0025,a6)                                      ;001076
        bne.s   AdvanceLFODelay                                 ;00107a
L00107c:                                                        ;00107c
        btst.b  #$05,($0016,a6)                                 ;00107c
        beq.s   L001086                                         ;001082
        bsr.s   L0010b4                                         ;001084
L001086:                                                        ;001086
        btst.b  #$06,($0016,a6)                                 ;001086
        beq.s   L001092                                         ;00108c
        bsr.w   L001116                                         ;00108e
L001092:                                                        ;001092
        rts                                                     ;001092

AdvanceLFODelay:                                                ;001094
        subq.b  #1,($0025,a6)                                   ;001094
        bne.s   L0010b2                                         ;001098
        btst.b  #$05,($0016,a6)                                 ;00109a
        beq.s   L0010a6                                         ;0010a0
        bsr.w   L001562                                         ;0010a2
L0010a6:                                                        ;0010a6
        btst.b  #$06,($0016,a6)                                 ;0010a6
        beq.s   L0010b2                                         ;0010ac
        bsr.w   L0015d0                                         ;0010ae
L0010b2:                                                        ;0010b2
        rts                                                     ;0010b2

L0010b4:                                                        ;0010b4
        move.l  ($0032,a6),d1                                   ;0010b4
        movea.l ($0026,a6),a0                                   ;0010b8
        jmp     (a0)                                            ;0010bc

L0010be:                                                        ;0010be
        add.l   d1,($0036,a6)                                   ;0010be
        subq.w  #1,($003e,a6)                                   ;0010c2
        bne.s   L0010d2                                         ;0010c6
        move.w  ($003c,a6),($003e,a6)                           ;0010c8
        neg.l   ($0036,a6)                                      ;0010ce
L0010d2:                                                        ;0010d2
        rts                                                     ;0010d2

L0010d4:                                                        ;0010d4
        move.l  d1,($0036,a6)                                   ;0010d4
        subq.w  #1,($003e,a6)                                   ;0010d8
        bne.s   L0010e8                                         ;0010dc
        move.w  ($003c,a6),($003e,a6)                           ;0010de
        neg.l   ($0032,a6)                                      ;0010e4
L0010e8:                                                        ;0010e8
        rts                                                     ;0010e8

L0010ea:                                                        ;0010ea
        add.l   d1,($0036,a6)                                   ;0010ea
        subq.w  #1,($003e,a6)                                   ;0010ee
        bne.s   L0010fe                                         ;0010f2
        move.w  ($003c,a6),($003e,a6)                           ;0010f4
        neg.l   ($0032,a6)                                      ;0010fa
L0010fe:                                                        ;0010fe
        rts                                                     ;0010fe

L001100:                                                        ;001100
        subq.w  #1,($003e,a6)                                   ;001100
        bne.s   L001114                                         ;001104
        bsr.s   L00117a                                         ;001106
        muls.w  d1,d0                                           ;001108
        move.l  d0,($0036,a6)                                   ;00110a
        move.w  ($003c,a6),($003e,a6)                           ;00110e
L001114:                                                        ;001114
        rts                                                     ;001114

L001116:                                                        ;001116
        move.w  ($0048,a6),d1                                   ;001116
        movea.l ($0040,a6),a0                                   ;00111a
        jmp     (a0)                                            ;00111e

L001120:                                                        ;001120
        add.w   d1,($004a,a6)                                   ;001120
        subq.w  #1,($004e,a6)                                   ;001124
        bne.s   L001136                                         ;001128
        move.w  ($004c,a6),($004e,a6)                           ;00112a
        move.w  ($0046,a6),($004a,a6)                           ;001130
L001136:                                                        ;001136
        rts                                                     ;001136

L001138:                                                        ;001138
        subq.w  #1,($004e,a6)                                   ;001138
        bne.s   L00114c                                         ;00113c
        move.w  ($004c,a6),($004e,a6)                           ;00113e
        add.w   d1,($004a,a6)                                   ;001144
        neg.w   ($0048,a6)                                      ;001148
L00114c:                                                        ;00114c
        rts                                                     ;00114c

L00114e:                                                        ;00114e
        add.w   d1,($004a,a6)                                   ;00114e
        subq.w  #1,($004e,a6)                                   ;001152
        bne.s   L001162                                         ;001156
        move.w  ($004c,a6),($004e,a6)                           ;001158
        neg.w   ($0048,a6)                                      ;00115e
L001162:                                                        ;001162
        rts                                                     ;001162

L001164:                                                        ;001164
        subq.w  #1,($004e,a6)                                   ;001164
        bne.s   L001178                                         ;001168
        bsr.s   L00117a                                         ;00116a
        muls.w  d0,d1                                           ;00116c
        move.w  ($004c,a6),($004e,a6)                           ;00116e
        move.w  d1,($004a,a6)                                   ;001174
L001178:                                                        ;001178
        rts                                                     ;001178

L00117a:                                                        ;00117a
        move.w  (-$107c,a5),d0                                  ;00117a
        mulu.w  #$c549,d0                                       ;00117e
        add.l   #$0000000c,d0                                   ;001182
        move.w  d0,(-$107c,a5)                                  ;001188
        lsr.l   #8,d0                                           ;00118c
        rts                                                     ;00118e

L001190:                                                        ;001190
        .dc.b   $12,$34                                         ;001190
L001192:                                                        ;001192
        lea.l   (chanel_sync_wait,pc),a0                        ;001192
        tst.b   (a0,d7.w)                                       ;001196
        bne.s   L00119e                                         ;00119a
        rts                                                     ;00119c

L00119e:                                                        ;00119e
        clr.b   (a0,d7.w)                                       ;00119e
        cmp.w   #$0009,d7                                       ;0011a2
        bcc.s   L0011ac                                         ;0011a6
        clr.b   ($27,a5,d7.w)                                   ;0011a8
L0011ac:                                                        ;0011ac
        andi.b  #$f7,($0017,a6)                                 ;0011ac
        bra.s   InitChannel                                     ;0011b2

L0011b4:                                                        ;0011b4
        btst.b  #$03,($0017,a6)                                 ;0011b4
        bne.s   L001192                                         ;0011ba
        btst.b  #$02,($0016,a6)                                 ;0011bc
        bne.s   L0011ce                                         ;0011c2
        subq.b  #1,($001b,a6)                                   ;0011c4
        bne.s   L0011ce                                         ;0011c8
        bsr.w   L000fe6                                         ;0011ca
L0011ce:                                                        ;0011ce
        subq.b  #1,($001a,a6)                                   ;0011ce
        bne.s   L001224                                         ;0011d2
InitChannel:                                                    ;0011d4
        movea.l (a6),a4                                         ;0011d4
        andi.b  #$7b,($0016,a6)                                 ;0011d6
L0011dc:                                                        ;0011dc
        moveq.l #$00,d0                                         ;0011dc
        moveq.l #$00,d1                                         ;0011de
        move.b  (a4)+,d0                                        ;0011e0
        move.b  d0,d1                                           ;0011e2
        bpl.s   L001216                                         ;0011e4
        cmp.b   #$e0,d0                                         ;0011e6
        bcc.s   L00122e                                         ;0011ea
        and.w   #$007f,d0                                       ;0011ec
        lsl.w   #6,d0                                           ;0011f0
        addq.w  #5,d0                                           ;0011f2
        add.w   ($0010,a6),d0                                   ;0011f4
        move.w  d0,($0012,a6)                                   ;0011f8
        ori.b   #$01,($0016,a6)                                 ;0011fc
        move.b  ($001f,a6),($0020,a6)                           ;001202
        moveq.l #$00,d0                                         ;001208
        move.b  (a4)+,d0                                        ;00120a
        move.b  ($001e,a6),d1                                   ;00120c
        bmi.s   L001226                                         ;001210
        mulu.w  d0,d1                                           ;001212
        lsr.w   #3,d1                                           ;001214
L001216:                                                        ;001216
        addq.w  #1,d1                                           ;001216
        move.b  d1,($001b,a6)                                   ;001218
        addq.w  #1,d0                                           ;00121c
        move.b  d0,($001a,a6)                                   ;00121e
        move.l  a4,(a6)                                         ;001222
L001224:                                                        ;001224
        rts                                                     ;001224

L001226:                                                        ;001226
        add.b   d0,d1                                           ;001226
        bcs.s   L001216                                         ;001228
        moveq.l #$00,d1                                         ;00122a
        bra.s   L001216                                         ;00122c

L00122e:                                                        ;00122e
        ext.w   d0                                              ;00122e
        not.w   d0                                              ;001230
        add.w   d0,d0                                           ;001232
        move.w  (CommandFuncs,pc,d0.w),d0                       ;001234
        pea.l   (L0011dc,pc)                                    ;001238
        jmp     (CommandFuncs,pc,d0.w)                          ;00123c

L001240:                                                        ;001240
        ext.w   d0                                              ;001240
        not.w   d0                                              ;001242
        add.w   d0,d0                                           ;001244
        move.w  (CommandFuncs,pc,d0.w),d0                       ;001246
        pea.l   (L00177a,pc)                                    ;00124a
        jmp     (CommandFuncs,pc,d0.w)                          ;00124e

CommandFuncs:                                                   ;001252
        .dc.w   SetTempoCommandFunc-CommandFuncs                ;001252
        .dc.w   WriteOPMCommandFunc-CommandFuncs                ;001254
        .dc.w   SetVoiceCommandFunc-CommandFuncs                ;001256
        .dc.w   PanCommandFunc-CommandFuncs                     ;001258
        .dc.w   VolumeCommandFunc-CommandFuncs                  ;00125a
        .dc.w   VolumeDownCommandFunc-CommandFuncs              ;00125c
        .dc.w   VolumeUpCommandFunc-CommandFuncs                ;00125e
        .dc.w   SetNoteLengthCommandFunc-CommandFuncs           ;001260
        .dc.w   SetLegatoCommandFunc-CommandFuncs               ;001262
        .dc.w   RepeatStartCommandFunc-CommandFuncs             ;001264
        .dc.w   RepeatEndCommandFunc-CommandFuncs               ;001266
        .dc.w   RepeatEscapeCommandFunc-CommandFuncs            ;001268
        .dc.w   DetuneCommandFunc-CommandFuncs                  ;00126a
        .dc.w   PortamentoCommandFunc-CommandFuncs              ;00126c
        .dc.w   PerformanceEndCommandFunc-CommandFuncs          ;00126e
        .dc.w   KeyOnDelayCommandFunc-CommandFuncs              ;001270
        .dc.w   SyncSendCommandFunc-CommandFuncs                ;001272
        .dc.w   SyncWaitCommandFunc-CommandFuncs                ;001274
        .dc.w   SetNoiseFreqCommandFunc-CommandFuncs            ;001276
        .dc.w   PitchModulationCommandFunc-CommandFuncs         ;001278
        .dc.w   AmplitudeModulationCommandFunc-CommandFuncs     ;00127a
        .dc.w   OPMLFOCommandFunc-CommandFuncs                  ;00127c
        .dc.w   SetLFOKeyOnDelayCommandFunc-CommandFuncs        ;00127e
        .dc.w   EnablePCM8CommandFunc-CommandFuncs              ;001280
        .dc.w   FadeOutCommandFunc-CommandFuncs                 ;001282
        .dc.w   EndPlayCommand-CommandFuncs                     ;001284
        .dc.w   EndPlayCommand-CommandFuncs                     ;001286
        .dc.w   EndPlayCommand-CommandFuncs                     ;001288
        .dc.w   EndPlayCommand-CommandFuncs                     ;00128a
        .dc.w   EndPlayCommand-CommandFuncs                     ;00128c
        .dc.w   EndPlayCommand-CommandFuncs                     ;00128e
        .dc.w   EndPlayCommand-CommandFuncs                     ;001290
SetTempoCommandFunc:                                            ;001292
        moveq.l #$12,d1                                         ;001292
        move.b  (a4)+,d2                                        ;001294
        move.b  d2,(-$0400,a5)                                  ;001296
        tst.b   (-$0404,a5)                                     ;00129a
        bne.s   L0012a4                                         ;00129e
        bra.w   L_WRITEOPM                                      ;0012a0

L0012a4:                                                        ;0012a4
        rts                                                     ;0012a4

WriteOPMCommandFunc:                                            ;0012a6
        move.b  (a4)+,d1                                        ;0012a6
        move.b  (a4)+,d2                                        ;0012a8
        cmp.b   #$12,d1                                         ;0012aa
        bne.s   L0012ba                                         ;0012ae
        tst.b   (-$0404,a5)                                     ;0012b0
        bne.s   L0012a4                                         ;0012b4
        move.b  d2,(-$0400,a5)                                  ;0012b6
L0012ba:                                                        ;0012ba
        bra.w   L_WRITEOPM                                      ;0012ba

SetVoiceCommandFunc:                                            ;0012be
        tst.b   ($0018,a6)                                      ;0012be
        bmi.s   L0012e0                                         ;0012c2
        move.b  (a4)+,d0                                        ;0012c4
        movea.l ($001c,a5),a0                                   ;0012c6
        bra.s   L0012d0                                         ;0012ca

L0012cc:                                                        ;0012cc
        lea.l   ($001a,a0),a0                                   ;0012cc
L0012d0:                                                        ;0012d0
        cmp.b   (a0)+,d0                                        ;0012d0
        bne.s   L0012cc                                         ;0012d2
        move.l  a0,($0004,a6)                                   ;0012d4
        ori.b   #$02,($0017,a6)                                 ;0012d8
        rts                                                     ;0012de

L0012e0:                                                        ;0012e0
        move.b  (a4)+,($0004,a6)                                ;0012e0
        rts                                                     ;0012e4

PanCommandFunc:                                                 ;0012e6
        tst.b   ($0018,a6)                                      ;0012e6
        bmi.s   L001302                                         ;0012ea
        move.b  ($001c,a6),d0                                   ;0012ec
        ror.w   #6,d0                                           ;0012f0
        move.b  (a4)+,d0                                        ;0012f2
        rol.w   #6,d0                                           ;0012f4
        move.b  d0,($001c,a6)                                   ;0012f6
        ori.b   #$04,($0017,a6)                                 ;0012fa
        rts                                                     ;001300

L001302:                                                        ;001302
        move.b  (a4)+,d0                                        ;001302
        beq.s   L00130c                                         ;001304
        cmp.b   #$03,d0                                         ;001306
        bne.s   L001310                                         ;00130a
L00130c:                                                        ;00130c
        eori.b  #$03,d0                                         ;00130c
L001310:                                                        ;001310
        andi.b  #$fc,($001c,a6)                                 ;001310
        or.b    d0,($001c,a6)                                   ;001316
        rts                                                     ;00131a

VolumeCommandFunc:                                              ;00131c
        move.b  (a4)+,($0022,a6)                                ;00131c
        ori.b   #$01,($0017,a6)                                 ;001320
        rts                                                     ;001326

VolumeDownCommandFunc:                                          ;001328
        move.b  ($0022,a6),d2                                   ;001328
        bmi.s   L00133c                                         ;00132c
        beq.s   L00133a                                         ;00132e
L001330:                                                        ;001330
        subq.b  #1,($0022,a6)                                   ;001330
        ori.b   #$01,($0017,a6)                                 ;001334
L00133a:                                                        ;00133a
        rts                                                     ;00133a

L00133c:                                                        ;00133c
        cmp.b   #-$01,d2                                        ;00133c
        bne.s   L001350                                         ;001340
        rts                                                     ;001342

VolumeUpCommandFunc:                                            ;001344
        move.b  ($0022,a6),d2                                   ;001344
        bmi.s   L00135c                                         ;001348
        cmp.b   #$0f,d2                                         ;00134a
        beq.s   L00135a                                         ;00134e
L001350:                                                        ;001350
        addq.b  #1,($0022,a6)                                   ;001350
        ori.b   #$01,($0017,a6)                                 ;001354
L00135a:                                                        ;00135a
        rts                                                     ;00135a

L00135c:                                                        ;00135c
        cmp.b   #$80,d2                                         ;00135c
        bne.s   L001330                                         ;001360
        rts                                                     ;001362

SetNoteLengthCommandFunc:                                       ;001364
        move.b  (a4)+,($001e,a6)                                ;001364
        rts                                                     ;001368

SetLegatoCommandFunc:                                           ;00136a
        ori.b   #$04,($0016,a6)                                 ;00136a
        rts                                                     ;001370

RepeatStartCommandFunc:                                         ;001372
        move.b  (a4)+,(a4)+                                     ;001372
        rts                                                     ;001374

RepeatEndCommandFunc:                                           ;001376
        moveq.l #$ff,d0                                         ;001376
        move.b  (a4)+,-(sp)                                     ;001378
        move.w  (sp)+,d0                                        ;00137a
        move.b  (a4)+,d0                                        ;00137c
        subq.b  #1,(-$01,a4,d0.l)                               ;00137e
        beq.s   L001398                                         ;001382
        tst.b   (-$0401,a5)                                     ;001384
        beq.s   L001396                                         ;001388
        cmpi.b  #$f1,(a4)                                       ;00138a
        bne.s   L001396                                         ;00138e
        tst.b   ($0001,a4)                                      ;001390
        beq.s   L0013e6                                         ;001394
L001396:                                                        ;001396
        adda.w  d0,a4                                           ;001396
L001398:                                                        ;001398
        rts                                                     ;001398

RepeatEscapeCommandFunc:                                        ;00139a
        moveq.l #$00,d0                                         ;00139a
        move.b  (a4)+,-(sp)                                     ;00139c
        move.w  (sp)+,d0                                        ;00139e
        move.b  (a4)+,d0                                        ;0013a0
        lea.l   (a4,d0.l),a0                                    ;0013a2
        moveq.l #$ff,d0                                         ;0013a6
        move.b  (a0)+,-(sp)                                     ;0013a8
        move.w  (sp)+,d0                                        ;0013aa
        move.b  (a0)+,d0                                        ;0013ac
        cmpi.b  #$01,(-$01,a0,d0.l)                             ;0013ae
        bne.s   L0013b8                                         ;0013b4
        movea.l a0,a4                                           ;0013b6
L0013b8:                                                        ;0013b8
        rts                                                     ;0013b8

DetuneCommandFunc:                                              ;0013ba
        move.b  (a4)+,-(sp)                                     ;0013ba
        move.w  (sp)+,d0                                        ;0013bc
        move.b  (a4)+,d0                                        ;0013be
        move.w  d0,($0010,a6)                                   ;0013c0
        rts                                                     ;0013c4

PortamentoCommandFunc:                                          ;0013c6
        move.b  (a4)+,-(sp)                                     ;0013c6
        move.w  (sp)+,d0                                        ;0013c8
        move.b  (a4)+,d0                                        ;0013ca
        ext.l   d0                                              ;0013cc
        asl.l   #8,d0                                           ;0013ce
        move.l  d0,($0008,a6)                                   ;0013d0
        ori.b   #$80,($0016,a6)                                 ;0013d4
        rts                                                     ;0013da

PerformanceEndCommandFunc:                                      ;0013dc
        moveq.l #$ff,d0                                         ;0013dc
        move.b  (a4)+,-(sp)                                     ;0013de
        beq.s   InitFadeout                                     ;0013e0
        move.w  (sp)+,d0                                        ;0013e2
        move.b  (a4)+,d0                                        ;0013e4
L0013e6:                                                        ;0013e6
        adda.l  d0,a4                                           ;0013e6
        move.w  (channel_enable,pc),d0                          ;0013e8
        bclr.l  d7,d0                                           ;0013ec
        move.w  d0,(-$03f2,a5)                                  ;0013ee
        and.w   (L001e06,pc),d0                                 ;0013f2
        bne.s   L00143e                                         ;0013f6
        tst.b   (-$03f4,a5)                                     ;0013f8
        bne.s   L001416                                         ;0013fc
        move.w  #$01ff,(-$03f2,a5)                              ;0013fe
        tst.b   (-$0418,a5)                                     ;001404
        beq.s   L001410                                         ;001408
        ori.w   #$fe00,(-$03f2,a5)                              ;00140a
L001410:                                                        ;001410
        addq.w  #1,($003a,a5)                                   ;001410
        bra.s   L00143e                                         ;001414

L001416:                                                        ;001416
        tst.b   (-$03f5,a5)                                     ;001416
        bne.s   L00143e                                         ;00141a
        move.w  #$01ff,(-$03f2,a5)                              ;00141c
        tst.b   (-$0418,a5)                                     ;001422
        beq.s   L00142e                                         ;001426
        ori.w   #$fe00,(-$03f2,a5)                              ;001428
L00142e:                                                        ;00142e
        subq.w  #1,(-$03ea,a5)                                  ;00142e
        bne.s   L00143e                                         ;001432
        move.w  #$0011,(-$03ee,a5)                              ;001434
        st.b    (-$03f5,a5)                                     ;00143a
L00143e:                                                        ;00143e
        rts                                                     ;00143e

InitFadeout:                                                    ;001440
        addq.w  #2,sp                                           ;001440
EndPlayCommand:                                                 ;001442
        lea.l   (EndPlayBytes,pc),a4                            ;001442
        move.w  (channel_enable,pc),d0                          ;001446
        bclr.l  d7,d0                                           ;00144a
        move.w  d0,(-$03f2,a5)                                  ;00144c
        move.w  (L001e06,pc),d0                                 ;001450
        bclr.l  d7,d0                                           ;001454
        move.w  d0,(-$0406,a5)                                  ;001456
        bne.s   L001490                                         ;00145a
        move.b  #$01,(-$03f9,a5)                                ;00145c
        tst.b   (-$0418,a5)                                     ;001462
        beq.s   L001472                                         ;001466
        move.w  #$01ff,d0                                       ;001468
        trap    #2                                              ;00146c
        clr.b   (-$0418,a5)                                     ;00146e
L001472:                                                        ;001472
        tst.b   (-$03f4,a5)                                     ;001472
        bne.s   L001480                                         ;001476
        move.w  #$ffff,($003a,a5)                               ;001478
        bra.s   L001490                                         ;00147e

L001480:                                                        ;001480
        move.w  #$ffff,(-$03ee,a5)                              ;001480
        st.b    (-$03f5,a5)                                     ;001486
        move.w  #$0037,(-$03f8,a5)      ;'7'                    ;00148a
L001490:                                                        ;001490
        rts                                                     ;001490

KeyOnDelayCommandFunc:                                          ;001492
        move.b  (a4)+,($001f,a6)                                ;001492
        rts                                                     ;001496

SyncSendCommandFunc:                                            ;001498
        moveq.l #$00,d0                                         ;001498
        move.b  (a4)+,d0                                        ;00149a
        lea.l   (chanel_sync_wait,pc),a0                        ;00149c
        st.b    (a0,d0.w)                                       ;0014a0
        cmp.w   #$0009,d0                                       ;0014a4
        bcc.s   L0014ae                                         ;0014a8
        st.b    ($27,a5,d0.w)                                   ;0014aa
L0014ae:                                                        ;0014ae
        rts                                                     ;0014ae

SyncWaitCommandFunc:                                            ;0014b0
        lea.l   (chanel_sync_wait,pc),a0                        ;0014b0
        tst.b   (a0,d7.w)                                       ;0014b4
        beq.s   L0014d0                                         ;0014b8
        clr.b   (a0,d7.w)                                       ;0014ba
        cmp.w   #$0009,d7                                       ;0014be
        bcc.s   L0014c8                                         ;0014c2
        clr.b   ($27,a5,d7.w)                                   ;0014c4
L0014c8:                                                        ;0014c8
        andi.b  #$f7,($0017,a6)                                 ;0014c8
        rts                                                     ;0014ce

L0014d0:                                                        ;0014d0
        ori.b   #$08,($0017,a6)                                 ;0014d0
        move.l  a4,(a6)                                         ;0014d6
        addq.w  #4,sp                                           ;0014d8
        rts                                                     ;0014da

SetNoiseFreqCommandFunc:                                        ;0014dc
        move.b  (a4)+,d2                                        ;0014dc
        tst.b   ($0018,a6)                                      ;0014de
        bmi.s   L0014ee                                         ;0014e2
        move.b  d2,($0026,a5)                                   ;0014e4
        moveq.l #$0f,d1                                         ;0014e8
        bra.w   L_WRITEOPM                                      ;0014ea

L0014ee:                                                        ;0014ee
        lsl.b   #2,d2                                           ;0014ee
        andi.b  #$03,($001c,a6)                                 ;0014f0
        or.b    d2,($001c,a6)                                   ;0014f6
        rts                                                     ;0014fa

PitchModulationCommandFunc:                                     ;0014fc
        ori.b   #$20,($0016,a6)                                 ;0014fc
        moveq.l #$00,d1                                         ;001502
        move.b  (a4)+,d1                                        ;001504
        bmi.s   L001576                                         ;001506
        move.w  d1,-(sp)                                        ;001508
        andi.b  #$03,d1                                         ;00150a
        add.w   d1,d1                                           ;00150e
        move.w  (L001588,pc,d1.w),d0                            ;001510
        lea.l   (L001588,pc,d0.w),a0                            ;001514
        move.l  a0,($0026,a6)                                   ;001518
        move.b  (a4)+,-(sp)                                     ;00151c
        move.w  (sp)+,d2                                        ;00151e
        move.b  (a4)+,d2                                        ;001520
        move.w  d2,($003c,a6)                                   ;001522
        cmp.b   #$02,d1                                         ;001526
        beq.s   L001536                                         ;00152a
        lsr.w   #1,d2                                           ;00152c
        cmpi.b  #$06,d1                                         ;00152e
        bne.s   L001536                                         ;001532
        moveq.l #$01,d2                                         ;001534
L001536:                                                        ;001536
        move.w  d2,($003a,a6)                                   ;001536
        move.b  (a4)+,-(sp)                                     ;00153a
        move.w  (sp)+,d0                                        ;00153c
        move.b  (a4)+,d0                                        ;00153e
        ext.l   d0                                              ;001540
        asl.l   #8,d0                                           ;001542
        move.w  (sp)+,d1                                        ;001544
        cmpi.b  #$04,d1                                         ;001546
        bcs.s   L001552                                         ;00154a
        asl.l   #8,d0                                           ;00154c
        andi.b  #$03,d1                                         ;00154e
L001552:                                                        ;001552
        move.l  d0,($002e,a6)                                   ;001552
        cmp.b   #$02,d1                                         ;001556
        beq.s   L00155e                                         ;00155a
        moveq.l #$00,d0                                         ;00155c
L00155e:                                                        ;00155e
        move.l  d0,($002a,a6)                                   ;00155e
L001562:                                                        ;001562
        move.w  ($003a,a6),($003e,a6)                           ;001562
        move.l  ($002e,a6),($0032,a6)                           ;001568
        move.l  ($002a,a6),($0036,a6)                           ;00156e
        rts                                                     ;001574

L001576:                                                        ;001576
        and.b   #$01,d1                                         ;001576
        bne.s   L001562                                         ;00157a
        andi.b  #$df,($0016,a6)                                 ;00157c
        clr.l   ($0036,a6)                                      ;001582
        rts                                                     ;001586

L001588:                                                        ;001588
        .dc.b   $fb,$36,$fb,$4c,$fb,$62,$fb,$78                 ;001588
AmplitudeModulationCommandFunc:                                 ;001590
        ori.b   #$40,($0016,a6)                                 ;001590
        moveq.l #$00,d2                                         ;001596
        move.b  (a4)+,d2                                        ;001598
        bmi.s   L0015e4                                         ;00159a
        add.w   d2,d2                                           ;00159c
        move.w  (L0015f6,pc,d2.w),d0                            ;00159e
        lea.l   (L0015f6,pc,d0.w),a0                            ;0015a2
        move.l  a0,($0040,a6)                                   ;0015a6
        move.b  (a4)+,-(sp)                                     ;0015aa
        move.w  (sp)+,d1                                        ;0015ac
        move.b  (a4)+,d1                                        ;0015ae
        move.w  d1,($004c,a6)                                   ;0015b0
        move.b  (a4)+,-(sp)                                     ;0015b4
        move.w  (sp)+,d0                                        ;0015b6
        move.b  (a4)+,d0                                        ;0015b8
        move.w  d0,($0044,a6)                                   ;0015ba
        btst.l  #$01,d2                                         ;0015be
        bne.s   L0015c6                                         ;0015c2
        muls.w  d1,d0                                           ;0015c4
L0015c6:                                                        ;0015c6
        neg.w   d0                                              ;0015c6
        bpl.s   L0015cc                                         ;0015c8
        moveq.l #$00,d0                                         ;0015ca
L0015cc:                                                        ;0015cc
        move.w  d0,($0046,a6)                                   ;0015cc
L0015d0:                                                        ;0015d0
        move.w  ($004c,a6),($004e,a6)                           ;0015d0
        move.w  ($0044,a6),($0048,a6)                           ;0015d6
        move.w  ($0046,a6),($004a,a6)                           ;0015dc
        rts                                                     ;0015e2

L0015e4:                                                        ;0015e4
        and.b   #$01,d2                                         ;0015e4
        bne.s   L0015d0                                         ;0015e8
        andi.b  #$bf,($0016,a6)                                 ;0015ea
        clr.w   ($004a,a6)                                      ;0015f0
        rts                                                     ;0015f4

L0015f6:                                                        ;0015f6
        .dc.b   $fb,$2a,$fb,$42,$fb,$58,$fb,$6e                 ;0015f6
OPMLFOCommandFunc:                                              ;0015fe
        move.b  (a4)+,d2                                        ;0015fe
        bmi.s   L00164a                                         ;001600
        andi.b  #$fd,($0016,a6)                                 ;001602
        bclr.l  #$06,d2                                         ;001608
        beq.s   L001614                                         ;00160c
        ori.b   #$02,($0016,a6)                                 ;00160e
L001614:                                                        ;001614
        move.b  ($09da),d0                                      ;001614
        and.b   #$c0,d0                                         ;001618
        or.b    d0,d2                                           ;00161c
        moveq.l #$1b,d1                                         ;00161e
        bsr.w   L_WRITEOPM                                      ;001620
        moveq.l #$18,d1                                         ;001624
        move.b  (a4)+,d2                                        ;001626
        bsr.w   L_WRITEOPM                                      ;001628
        moveq.l #$19,d1                                         ;00162c
        move.b  (a4)+,d2                                        ;00162e
        bsr.w   L_WRITEOPM                                      ;001630
        move.b  (a4)+,d2                                        ;001634
        bsr.w   L_WRITEOPM                                      ;001636
        move.b  (a4)+,d2                                        ;00163a
        move.b  d2,($0021,a6)                                   ;00163c
L001640:                                                        ;001640
        moveq.l #$38,d1                 ;'8'                    ;001640
        add.b   ($0018,a6),d1                                   ;001642
        bra.w   L_WRITEOPM                                      ;001646

L00164a:                                                        ;00164a
        and.b   #$01,d2                                         ;00164a
        beq.s   L001640                                         ;00164e
        move.b  ($0021,a6),d2                                   ;001650
        bra.s   L001640                                         ;001654

SetLFOKeyOnDelayCommandFunc:                                    ;001656
        move.b  (a4)+,($0024,a6)                                ;001656
        rts                                                     ;00165a

EnablePCM8CommandFunc:                                          ;00165c
        movea.l ($0088),a0                                      ;00165c
        cmpa.l  #$00f00000,a0                                   ;001660
        bcc.s   L001692                                         ;001666
        cmpi.l  #$50434d34,(-$0008,a0)  ;'PCM4'                 ;001668
        beq.s   L00167c                                         ;001670
        cmpi.l  #$50434d38,(-$0008,a0)  ;'PCM8'                 ;001672
        bne.s   L001692                                         ;00167a
L00167c:                                                        ;00167c
        st.b    (-$0418,a5)                                     ;00167c
        move.w  #$01fe,d0                                       ;001680
        trap    #2                                              ;001684
        ori.w   #$fe00,(-$03f2,a5)                              ;001686
        ori.w   #$fe00,(-$0406,a5)                              ;00168c
L001692:                                                        ;001692
        rts                                                     ;001692

FadeOutCommandFunc:                                             ;001694
        moveq.l #$00,d0                                         ;001694
        move.b  (a4)+,d0                                        ;001696
        cmp.w   #$0007,d0                                       ;001698
        bcc.w   EndPlayCommand                                  ;00169c
        add.w   d0,d0                                           ;0016a0
        move.w  (FadeOutFuncs,pc,d0.w),d0                       ;0016a2
        jmp     (FadeOutFuncs,pc,d0.w)                          ;0016a6

FadeOutFuncs:                                                   ;0016aa
        .dc.w   EndPlayCommand-FadeOutFuncs                     ;0016aa
        .dc.w   EnableFadeout-FadeOutFuncs                      ;0016ac
        .dc.w   AdvanceChannel-FadeOutFuncs                     ;0016ae
        .dc.w   L0016fa-FadeOutFuncs                            ;0016b0
        .dc.w   L00170e-FadeOutFuncs                            ;0016b2
        .dc.w   L00178a-FadeOutFuncs                            ;0016b4
        .dc.w   L0017a0-FadeOutFuncs                            ;0016b6
EnableFadeout:                                                  ;0016b8
        moveq.l #$00,d0                                         ;0016b8
        move.b  (a4)+,d0                                        ;0016ba
        move.w  d0,(-$03ee,a5)                                  ;0016bc
        st.b    (-$03f5,a5)                                     ;0016c0
        rts                                                     ;0016c4

AdvanceChannel:                                                 ;0016c6
        movea.l ($0088),a0                                      ;0016c6
        move.l  (-$0008,a0),d0                                  ;0016ca
        cmp.l   #$50434d38,d0           ;'PCM8'                 ;0016ce
        beq.s   L0016e2                                         ;0016d4
        cmp.l   #$50434d34,d0           ;'PCM4'                 ;0016d6
        beq.s   L0016e2                                         ;0016dc
        addq.w  #6,a4                                           ;0016de
        rts                                                     ;0016e0

L0016e2:                                                        ;0016e2
        move.b  (a4)+,-(sp)                                     ;0016e2
        move.w  (sp)+,d0                                        ;0016e4
        move.b  (a4)+,d0                                        ;0016e6
        move.b  (a4)+,-(sp)                                     ;0016e8
        move.w  (sp)+,d1                                        ;0016ea
        move.b  (a4)+,d1                                        ;0016ec
        swap.w  d1                                              ;0016ee
        move.b  (a4)+,-(sp)                                     ;0016f0
        move.w  (sp)+,d1                                        ;0016f2
        move.b  (a4)+,d1                                        ;0016f4
        trap    #2                                              ;0016f6
        rts                                                     ;0016f8

L0016fa:                                                        ;0016fa
        tst.b   (a4)+                                           ;0016fa
        beq.s   L001706                                         ;0016fc
        ori.b   #$10,($0016,a6)                                 ;0016fe
        rts                                                     ;001704

L001706:                                                        ;001706
        andi.b  #$ef,($0016,a6)                                 ;001706
        rts                                                     ;00170c

L00170e:                                                        ;00170e
        move.b  (a4)+,d0                                        ;00170e
        movem.l d7/a6,-(sp)                                     ;001710
        lea.l   (-$02d0,a5),a6                                  ;001714
        move.w  d0,d7                                           ;001718
        cmp.b   #$09,d0                                         ;00171a
        bcs.s   L001724                                         ;00171e
        lea.l   (-$0918,a5),a6                                  ;001720
L001724:                                                        ;001724
        mulu.w  #$0050,d0                                       ;001724
        adda.w  d0,a6                                           ;001728
        move.l  (a6),-(sp)                                      ;00172a
        andi.b  #$7b,($0016,a6)                                 ;00172c
        moveq.l #$00,d0                                         ;001732
        moveq.l #$00,d1                                         ;001734
        move.b  (a4)+,d0                                        ;001736
        move.b  d0,d1                                           ;001738
        bpl.s   L00176e                                         ;00173a
        cmp.b   #$e0,d0                                         ;00173c
        bcc.w   L001240                                         ;001740
        and.w   #$007f,d0                                       ;001744
        lsl.w   #6,d0                                           ;001748
        addq.w  #5,d0                                           ;00174a
        add.w   ($0010,a6),d0                                   ;00174c
        move.w  d0,($0012,a6)                                   ;001750
        ori.b   #$01,($0016,a6)                                 ;001754
        move.b  ($001f,a6),($0020,a6)                           ;00175a
        moveq.l #$00,d0                                         ;001760
        move.b  (a4)+,d0                                        ;001762
        move.b  ($001e,a6),d1                                   ;001764
        bmi.s   L001782                                         ;001768
        mulu.w  d0,d1                                           ;00176a
        lsr.w   #3,d1                                           ;00176c
L00176e:                                                        ;00176e
        addq.w  #1,d1                                           ;00176e
        move.b  d1,($001b,a6)                                   ;001770
        addq.w  #1,d0                                           ;001774
        move.b  d0,($001a,a6)                                   ;001776
L00177a:                                                        ;00177a
        move.l  (sp)+,(a6)                                      ;00177a
        movem.l (sp)+,d7/a6                                     ;00177c
        rts                                                     ;001780

L001782:                                                        ;001782
        add.b   d0,d1                                           ;001782
        bcs.s   L00176e                                         ;001784
        moveq.l #$00,d1                                         ;001786
        bra.s   L00176e                                         ;001788

L00178a:                                                        ;00178a
        move.b  (a4)+,d0                                        ;00178a
        move.b  d0,d1                                           ;00178c
        bsr.w   L001216                                         ;00178e
        andi.b  #$fe,($0016,a6)                                 ;001792
        bsr.w   SetPCMVolume                                    ;001798
        addq.w  #4,sp                                           ;00179c
        rts                                                     ;00179e

L0017a0:                                                        ;0017a0
        tst.b   (a4)+                                           ;0017a0
        beq.s   L0017ac                                         ;0017a2
        ori.b   #$80,($0017,a6)                                 ;0017a4
        rts                                                     ;0017aa

L0017ac:                                                        ;0017ac
        andi.b  #$7f,($0017,a6)                                 ;0017ac
        rts                                                     ;0017b2

L_WRITEOPM:                                                     ;0017b4
        ori.w   #$0300,sr                                       ;0017b4
        tst.b   ($00e90003)                                     ;0017b8
        bmi.s   L_WRITEOPM                                      ;0017be
        move.b  d1,($00e90001)                                  ;0017c0
        and.w   #$00ff,d1                                       ;0017c6
        lea.l   (L001e3c,pc),a2                                 ;0017ca
        move.b  d2,(a2,d1.w)                                    ;0017ce
        move.b  d2,($00e90003)                                  ;0017d2
        andi.w  #$faff,sr                                       ;0017d8
        cmp.b   #$1b,d1                                         ;0017dc
        beq.s   L0017e4                                         ;0017e0
        rts                                                     ;0017e2

L0017e4:                                                        ;0017e4
        move.b  d2,($09da)                                      ;0017e4
        rts                                                     ;0017e8

EntryPoint:                                                     ;0017ea
        clr.l   -(sp)                                           ;0017ea
        DOS     _SUPER                                          ;0017ec
        pea.l   (VersionString,pc)                              ;0017ee
        DOS     _PRINT                                          ;0017f2
        lea.l   (L00220c,pc),a5                                 ;0017f4
        move.w  #$0001,(-$03fa,a5)                              ;0017f8
        clr.l   (-$0404,a5)                                     ;0017fe
        move.l  ($0008,a0),(-$065c,a5)                          ;001802
        move.l  #$00010000,($0014,a5)                           ;001808
        move.l  #$0004e000,($0018,a5)                           ;001810
        move.l  #$00000600,(-$0664,a5)                          ;001818
        clr.b   ($0024,a5)                                      ;001820
        clr.b   ($0025,a5)                                      ;001824
        addq.w  #1,a2                                           ;001828
        bsr.s   ParseCmdLineArgs                                ;00182a
        lea.l   (L002248,pc),a4                                 ;00182c
        lea.l   (mdx_data,pc),a1                                ;001830
        move.l  a4,(a1)                                         ;001834
        adda.l  ($0014,a5),a4                                   ;001836
        move.l  a4,($0004,a1)                                   ;00183a
        adda.l  ($0018,a5),a4                                   ;00183e
        move.l  a4,(-$0660,a5)                                  ;001842
        adda.l  (-$0664,a5),a4                                  ;001846
        cmpa.l  ($0008,a0),a4                                   ;00184a
        bhi.w   PrintInsufficientMemory                         ;00184e
        suba.l  a0,a0                                           ;001852
        move.l  ($0090,a0),(a5)                                 ;001854
        move.l  ($010c,a0),($0004,a5)                           ;001858
        lea.l   (L00000c,pc),a1                                 ;00185e
        move.l  a1,($0090,a0)                                   ;001862
        lea.l   (L000000,pc),a1                                 ;001866
        suba.l  a1,a4                                           ;00186a
        clr.w   -(sp)                                           ;00186c
        move.l  a4,-(sp)                                        ;00186e
        DOS     _KEEPPR                                         ;001870

L001872:                                                        ;001872
        movea.l ($0090),a4                                      ;001872
        cmpa.l  #$00fe0000,a4                                   ;001876
        bcc.s   L001890                                         ;00187c
        subq.w  #8,a4                                           ;00187e
        lea.l   (L000004,pc),a3                                 ;001880
        move.w  #$0004,d0                                       ;001884
L001888:                                                        ;001888
        cmpm.b  (a3)+,(a4)+                                     ;001888
        bne.s   L001890                                         ;00188a
        dbra.w  d0,L001888                                      ;00188c
L001890:                                                        ;001890
        rts                                                     ;001890

ParseCmdLineArgs:                                               ;001892
        move.b  (a2)+,d0                                        ;001892
        bne.s   L00189e                                         ;001894
        bsr.s   L001872                                         ;001896
        beq.w   PrintAlreadyLoaded                              ;001898
        rts                                                     ;00189c

L00189e:                                                        ;00189e
        cmp.b   #$20,d0                 ;' '                    ;00189e
        beq.s   ParseCmdLineArgs                                ;0018a2
        cmp.b   #$09,d0                                         ;0018a4
        beq.s   ParseCmdLineArgs                                ;0018a8
        cmp.b   #$2d,d0                 ;'-'                    ;0018aa
        beq.s   L0018b8                                         ;0018ae
        cmp.b   #$2f,d0                 ;'/'                    ;0018b0
        bne.w   PrintHelp                                       ;0018b4
L0018b8:                                                        ;0018b8
        move.b  (a2)+,d0                                        ;0018b8
        beq.w   PrintHelp                                       ;0018ba
        or.b    #$20,d0                                         ;0018be
        cmp.b   #$72,d0                 ;'r'                    ;0018c2
        bne.s   L0018e8                                         ;0018c6
        bsr.s   L001872                                         ;0018c8
        bne.w   PrintNotLoaded                                  ;0018ca
        moveq.l #$00,d0                                         ;0018ce
        trap    #4                                              ;0018d0
        tst.l   d0                                              ;0018d2
        bne.s   L0018e2                                         ;0018d4
        pea.l   (MXDRVReleasedString,pc)                        ;0018d6
L0018da:                                                        ;0018da
        DOS     _PRINT                                          ;0018da
        addq.w  #4,sp                                           ;0018dc
        clr.w   -(sp)                                           ;0018de
        DOS     _EXIT2                                          ;0018e0

L0018e2:                                                        ;0018e2
        pea.l   (CouldNotReleaseString,pc)                      ;0018e2
        bra.s   L0018da                                         ;0018e6

L0018e8:                                                        ;0018e8
        bsr.s   L001872                                         ;0018e8
        beq.w   PrintAlreadyLoaded                              ;0018ea
        cmp.b   #$6d,d0                 ;'m'                    ;0018ee
        bne.s   L001904                                         ;0018f2
        cmpi.b  #$3a,(a2)               ;':'                    ;0018f4
        bne.s   L0018fc                                         ;0018f8
        addq.w  #1,a2                                           ;0018fa
L0018fc:                                                        ;0018fc
        bsr.s   L001966                                         ;0018fc
        move.l  d0,($0014,a5)                                   ;0018fe
        bra.s   ParseCmdLineArgs                                ;001902

L001904:                                                        ;001904
        cmp.b   #$70,d0                 ;'p'                    ;001904
        bne.s   L00191c                                         ;001908
        cmpi.b  #$3a,(a2)               ;':'                    ;00190a
        bne.s   L001912                                         ;00190e
        addq.w  #1,a2                                           ;001910
L001912:                                                        ;001912
        bsr.s   L001966                                         ;001912
        move.l  d0,($0018,a5)                                   ;001914
        bra.w   ParseCmdLineArgs                                ;001918

L00191c:                                                        ;00191c
        cmp.b   #$62,d0                 ;'b'                    ;00191c
        bne.s   L00193c                                         ;001920
        cmpi.b  #$3a,(a2)               ;':'                    ;001922
        bne.s   L00192a                                         ;001926
        addq.w  #1,a2                                           ;001928
L00192a:                                                        ;00192a
        bsr.s   L001972                                         ;00192a
        move.l  d0,-(sp)                                        ;00192c
        add.l   d0,d0                                           ;00192e
        add.l   (sp)+,d0                                        ;001930
        lsl.l   #8,d0                                           ;001932
        move.l  d0,(-$0664,a5)                                  ;001934
        bra.w   ParseCmdLineArgs                                ;001938

L00193c:                                                        ;00193c
        cmpi.b  #$66,d0                 ;'f'                    ;00193c
        bne.s   PrintHelp                                       ;001940
        cmpi.b  #$3a,(a2)               ;':'                    ;001942
        bne.s   L00194a                                         ;001946
        addq.w  #1,a2                                           ;001948
L00194a:                                                        ;00194a
        move.l  a2,-(sp)                                        ;00194a
        bsr.s   L001972                                         ;00194c
        cmpa.l  (sp)+,a2                                        ;00194e
        beq.s   L00195a                                         ;001950
        andi.l  #$00007fff,d0                                   ;001952
        bra.s   L00195c                                         ;001958

L00195a:                                                        ;00195a
        moveq.l #$11,d0                                         ;00195a
L00195c:                                                        ;00195c
        move.w  d0,(L000b90+$000002)                            ;00195c
        bra.w   ParseCmdLineArgs                                ;001962

L001966:                                                        ;001966
        bsr.s   L001972                                         ;001966
        tst.l   d0                                              ;001968
        bmi.s   PrintHelp                                       ;00196a
        moveq.l #$0a,d1                                         ;00196c
        lsl.l   d1,d0                                           ;00196e
        rts                                                     ;001970

L001972:                                                        ;001972
        moveq.l #$00,d0                                         ;001972
L001974:                                                        ;001974
        moveq.l #$00,d1                                         ;001974
        move.b  (a2),d1                                         ;001976
        sub.b   #$30,d1                 ;'0'                    ;001978
        bcs.s   L001992                                         ;00197c
        cmp.b   #$09,d1                                         ;00197e
        bhi.s   L001992                                         ;001982
        add.l   d0,d0                                           ;001984
        add.l   d0,d1                                           ;001986
        add.l   d0,d0                                           ;001988
        add.l   d0,d0                                           ;00198a
        add.l   d1,d0                                           ;00198c
        addq.w  #1,a2                                           ;00198e
        bra.s   L001974                                         ;001990

L001992:                                                        ;001992
        rts                                                     ;001992

PrintNotLoaded:                                                 ;001994
        pea.l   (MXDRVNotLoadedString,pc)                       ;001994
        bra.s   L0019aa                                         ;001998

PrintAlreadyLoaded:                                             ;00199a
        pea.l   (MXDRVAlreadyLoadedString,pc)                   ;00199a
        bra.s   L0019aa                                         ;00199e

PrintInsufficientMemory:                                        ;0019a0
        pea.l   (InsufficientMemoryString,pc)                   ;0019a0
        bra.s   L0019aa                                         ;0019a4

PrintHelp:                                                      ;0019a6
        pea.l   (HelpString,pc)                                 ;0019a6
L0019aa:                                                        ;0019aa
        DOS     _PRINT                                          ;0019aa
        move.w  #$ffff,-(sp)                                    ;0019ac
        DOS     _EXIT2                                          ;0019b0

EndPlayBytes:
        .dc.b   $7f,$f1,$00
VersionString:
        .dc.b   'X68k MXDRV music driver version 2.06+17 Rel.X5-S (c)1988-92'
        .dc.b   ' milk.,K.MAEKAWA, Missy.M, Yatsube',$0d,$0a,$00
HelpString:
        .dc.b   'Usage: mxdrv [switch]',$0d,$0a
        .dc.b   $09,'-m:<num> MML buffer size(Kbytes)',$09,$09,'[Default 64]',$0d,$0a
        .dc.b   $09,'-p:<num> PCM buffer size(Kbytes)',$09,$09,'[Default 312]',$0d,$0a
        .dc.b   $09,'-b:<num> LINK buffer size(banks)',$09,$09,'[Default 2]',$0d,$0a
        .dc.b   $09,'-f:<num> FADEOUT SPEED (FAST 0 - 32767 SLOW)',$09,'[Default 23]',$0d,$0a
        .dc.b   $09,'-r       Release mxdrv',$0d,$0a,$00
InsufficientMemoryString:
        .dc.b   $09,'Insufficient memory',$0d,$0a,$00
MXDRVReleasedString:
        .dc.b   $09,'mxdrv released',$0d,$0a,$00
MXDRVAlreadyLoadedString:
        .dc.b   $09,'mxdrv is already loaded',$0d,$0a,$00
MXDRVNotLoadedString:
        .dc.b   $09,'mxdrv is not loaded',$0d,$0a,$00
CouldNotReleaseString:
        .dc.b   $09,'Could not release mxdrv',$0d,$0a,$00

        .bss

L001ba6:
        .ds.b   14
pcm_work_area:
        .ds.b   16
L001bc4:
        .ds.b   560
pcm8_enable:
        .ds.b   2
chanel_sync_wait:
        .ds.b   16
L001e06:
        .ds.w   1
disable_timer_writes:
        .ds.b   2
ignore_keys:
        .ds.b   2
opm_tempo:
        .ds.b   2
L001e0e:
        .ds.b   4
paused:
        .ds.b   1
ended:
        .ds.b   1
fadeout_offset:
        .ds.b   3
fadeout_enable:
        .ds.b   2
L001e19:
        .ds.b   1
channel_enable:
        .ds.w   1
channel_mask:
        .ds.w   1
L001e1e:
        .ds.b   6
L001e24:
        .ds.l   1
L001e28:
        .ds.l   1
L001e2c:
        .ds.l   1
L001e30:
        .ds.l   1
mdx_data:
        .ds.l   1
pdx_data:
        .ds.l   1
L001e3c:
        .ds.b   256
L001f3c:
        .ds.b   720
L00220c:
        .ds.b   12
mdx_title:
        .ds.l   1
pdx_filename:
        .ds.l   1
mdx_size:
        .ds.l   1
pdx_size:
        .ds.l   1
voice_data:
        .ds.l   1
pdx_sample_indices:
        .ds.b   16
L00223c:
        .ds.b   10
loop_count:
        .ds.w   1
L002248:

        .end    EntryPoint
