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

Func:   Output data of single memory area to ADPCM.

Call:   d0.w = $0000 + channel number(0〜7)

        d1.l = volume / frequency / localization

            bit 23〜16 : Volume (-1,0〜15) (*1)
                If -1 is specified, the previous value is retained.
                Original volume is 8.
                It corresponds to 1 step = 2 dB (-16dB〜+14dB)。

            bit 15〜 8 : frequency, data format (-1,0〜4,5,6)
                If -1 is specified, the previous value is retained.
                0〜4 are ADPCM, and the reproduction frequency is as follows.
                    0 : 3.9KHz  3 : 10.4KHz
                    1 : 5.2KHz  4 : 15.6KHz
                    2 : 7.8KHz
                5 is 16 bit PCM, and 6 is 8 bit PCM. (*2)
                    Refer to "About PCM data format" for these.

            bit  7〜 0 : pan (-1,0〜3)
                If -1 is specified, the previous value is retained.
                Other values ​​are as follows.
                    0 : Stop(*3)     2 : Right output
                    1 : Left output      3 : Stereo output
                Note that the localization is common to all channels,
                and the last specified value is valid except for 0.

        d2.l = Data length
            Regardless of the data format, specify it in byte units.
            <0 : Query data length (equivalent to function code $008x)
            =0 : Channel stop (*4)

        a1.l = Data block start address

Ret:    d0.l = 0        Successful completion

Note:
(*1) In the single sound reproduction mode, regardless of the setting, it is output at the original volume.
(*2) It is not output in the single sound reproduction mode.
(*3) The volume / frequency is changed. Positioning is not changed.
(*4) Volume / frequency / localization is not changed.

----------------------------------------
Array chain output       Func code $001x
----------------------------------------

Func:   Data of a plurality of memory areas indirectly designated by the chain table is outputted to the ADPCM.

Call:   d0.w = $0010 + Channel number(0〜7)
        d1.l = Volume / frequency / localization
            >> Reference normal output ($000x)
        d2.l = Number of chain tables
            <0 : Data length query(Equivalent to func code $008x)
            =0 : Channel stop(*4)
        a1.l = Chain table start address

        *Explanation about the chain table is omitted.

Ret:    d0.l = 0        Success

----------------------------------------
Link chain output       Func code $002x
----------------------------------------

Func:   And outputs data of a plurality of memory areas indirectly designated by the link chain table to the ADPCM.

Call:   d0.w = $0020 + Channel number(0〜7)
        d1.l = Volume / frequency / localization
            >> Reference normal output ($000x)
        a1.l = Link chain table start address

        * Explanation about the link chain table is omitted.

Ret:    d0.l = 0        Success

----------------------------------------
Operation mode change   Func code $007x
----------------------------------------

Func:   Change the volume / frequency / pan of the active channel.

Call:   d0.w = $0070 + Channel number(0〜7)
        d1.l = Volume / frequency / localization
            >> Reference normal output ($000x)

Ret:    d0.l = 0        Success

----------------------------------------
Data length query        Func code $008x
----------------------------------------

Func:   Get the remaining data length of the active channel.

Call:   d0.w = $0080 + Channel number(0〜7)

Ret:    d0.l = 0        Channel is stopped
        d0.l > 0        Indicates the remaining data length (byte) during normal output in the multiple reproduction mode.
        d0.l = -1       Array chain outputting
        d0.l = -2       Link chain outputting
        d0.l = -3       In mono tone reproduction mode, normal output

----------------------------------------
Operational mode query   Func code $009x
----------------------------------------

Func:   Get the volume / frequency / localization of the active channel.

Call:   d0.w = $0090 + Channel number(0〜7)

Ret:    d0.l = Volume / frequency / localization
            >> Reference normal output ($000x)
            Localization always takes values 1 to 3.
            Bits 31 to 24 are 0.

----------------------------------------
End                      Func code $0100
----------------------------------------

Func:   The chain operation of all the channels is canceled and the channel operation is stopped as soon as the output of the currently output data block is completed.

        If it is paused, immediately stop the operation of all channels.

Call:   d0.w = $0100

Ret:    d0.l = 0        Success

Note:   Note that it is not an immediate termination.
        If you want to terminate completely immediately, issue this call after pause ($ 0101).
        During single tone IOCS output, this call is ignored.

----------------------------------------
Pause                    Func code $0101
----------------------------------------

Func:   The output of all channels is immediately stopped, and the pause state is started.

        When channel output or termination is performed in the suspended state, the operation of all channels is aborted.

Call:   d0.w = $0101

Ret:    d0.l = 0        Success

Note:   During single tone IOCS output, this call is ignored.

----------------------------------------
Unpause                 Func code $0102
----------------------------------------

Func:   Cancel the paused state.

        Invalid after discontinuation.

Call:   d0.w = $0102

Ret:    d0.l = 0        Success

Note:   During single tone IOCS output, this call is ignored.

----------------------------------------
MPU / MFP mask settings  Func code $01FB
----------------------------------------

Func:   PCM8 Sets MPU / MFP interrupt mask during interrupt.

Call:   d0.w = $01FB
        d1.l = Func/Mask value
            < 0 : Mask query only
            >=0 : Mask setting
                bit 18〜16 : MPU mask
                bit 15〜 8 : IMRA mask
                bit  7〜 0 : IMRB mask

Ret:    d0.l = (Previous) Mask value
                bit 31〜19 are 0。

Note:   It should not be used in general programs.

        Default value is %011_11011111_00000000 。
        No function call should be made from within unmasked interrupt handling.

        Please understand the contents before using it.

----------------------------------------
Multiple / single tone mode setting Func code $01FC
----------------------------------------

Func:   Enable / disable multiple playback.

Call:   d0.w = $01FC
        d1.w = Authorization flag
            -1 : Query only
             0 : Ban
             1 : Authorization
             2 : Allow only function call

Ret:    d0.l = Actual allowed state
             0 : Prohibited
             1 : Allowing
             2 : Only function calls are permitted

Note:   It should not be used in general programs.

        While multiple playback is prohibited, the output by the function is effective afterward. Furthermore, the IOCS output has priority over the function output.

        This call is independent of the permission / prohibition from the PCM 8 command line, and only when both settings are "permitted" it is set to the multiple reproduction mode.

        The return value reflects both settings.

----------------------------------------
(reserved)              Func code $01FD
----------------------------------------

        This call is reserved.

----------------------------------------
Occupancy                Func code $01FE
----------------------------------------

Func:   Occupy PCM8 and prohibit resident release.

Call:   d0.w = $01FE

Ret:    d0.l = 0        Occupied
        d0.l = -1       Already occupied

Note:   It should not be used in general programs.

----------------------------------------
Obligation               Func code $01FF
----------------------------------------

Func:   Unlock PCM8 and enable resident release.

Call:   d0.w = $01FF

Ret:    d0.l = 0        Dismissed
        d0.l = -1       Not occupied

Note:   It should not be used in general programs.

----------------------------------------
Resident reset           Func code $FFFF
----------------------------------------

Func:   Releasing the PCM 8 resident.

Call:   d0.w = $FFFF

Ret:    d0.l = 0        Resident cancellation was executed
        d0.l = -1       It was occupied and could not be unlocked
        d0.l = -2       The interrupt vector etc. used in PCM8 has been changed, and it can not be canceled

Note:   It should not be used in general programs.

        There are 7 vectors used in PCM8.

        VECTOR= ADDR=== Content==========
        $022    $000088 TRAP #2
        $06A    $0001A8 DMAC ch.3 Interrupt
        $06B    $0001AC DMAC ch.3 Error interrupt
        $160    $000580 IOCS _ADPCMOUT
        $162    $000588 IOCS _ADPCMAOT
        $164    $000590 IOCS _ADPCMLOT
        $167    $00059C IOCS _ADPCMMOD


-- ＩＯＣＳ Call ( TRAP #15 )-------------------------------------------------


        Apart from the PCM 8 function call, IOCS calls are prepared so that
        it can be used in almost the same procedure as before.

　Supported call:

        $60 : _ADPCMOUT         Chainless output
        $62 : _ADPCMAOT         Array chain output
        $64 : _ADPCMLOT         Link array chain output
        $67 : _ADPCMMOD         ADPCM execution control

        ADPCM related calls ($61, $63, $65, $66) other than the above
        remain as they were before being resident.

　Difference from original IOCS:

　　（Multiple playback mode）

　　　・When outputting again within the sound being emitted, it synthesizes
        with the output of the other channels without waiting for output
        completion.

        In this case, use unused channels arbitrarily. The direction to search
        is from back to front (channels 8 · 7 · · · 1).
        When there is no vacancy, the channel with the shortest remaining time
        among the channels without output is crushed and output.
        Ignore the call if there is no corresponding channel.

　　　・With a $60 call, we return immediately even if the length is over $FF00.

　　　・Bread will follow the latest designation.

        Reverberation during output is also affected. However, designation of output cutting is completely ignored.

　　　・$67 End of call specification ends the chain operation.

        The currently output data block is output as it is. To terminate immediately, designate cancellation. It is also possible to resume.

        After stopping designation, when designation of termination or reproduction of another sound is made, all the original outputs are canceled.

　　（Single sound reproduction mode）

　　　・If outputting is performed again while the previous sound is being emitted,
        the output of the preceding sound is canceled and outputted without waiting for output completion.

　　　・With a $60 call, we return immediately even if the length is over $FF00.

　Other:

　　　・For IOCS calls, 16 bit PCM, 8 bit PCM can not be output.
　　　・The volume of the IOCS call is fixed at 8 (original sound).


-- How to determine the resident status of PCM8 ------------------------------------


        When PCM8 function call is used, it is necessary to make a resident judgment in advance.

        For resident judgment, the following method is recommended.

　Method 1: Trap vector acquisition method

        Check the 8 bytes before the content of the exception vector $22 (= ($0088).l) (= TRAP #2 entry).

        If 5 bytes from the top are 'PCM4/' or 'PCM8/', it resides.
        The following 3 bytes are versions. ex) version 0.45 = 'PCM8/045'

        This judgment method is valid for PCM 4/8 with version 0.11 or higher.

        If you want to reliably determine whether it is PCM 4 or PCM 8, please check with the version (PCM 4 is less than 030).
        The header after version 0.40 is unified to the format 'PCM8/???'.

　Method 2: IOCS call method (simplified method)

        Set constant 'PCM8' to d1.l and issue IOCS $ 67 (_ADPCMMOD).
        If the return value d0.l > 0, the PCM8 is resident and the version is d0.l/100 (decimal).

        This judgment method is valid for PCM8 version 0.45 or higher.


-- other noteworthy things ----------------------------------------------------


　・About OPM register access

        PCM8 may access the OPM register.

        Since the access register is $1B and it decides the value by referring
        to the system work $09DA, it is necessary for other applications to
        avoid misunderstanding between system work and actual setting contents.

　・About PCM 8 built-in control

        If it is not convenient for the sound source driver to perform ADPCM
        processing independently, if it is not convenient to incorporate the
        PCM 8 later, if a dummy address is set in the entry ( = ($0088).l)
        of TRAP #2, PCM8 You can prevent the incorporation of.

　・About the position of the PCM8 header

        As of version 0.48, the identification header of PCM8 is at the head
        of the load address, but since it is planned to change in the future,
        please avoid coding such as back calculation of MCB / PSP from header
        address, for example.

-------------------------------------------------------------------------------
```
