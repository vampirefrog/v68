RUN68=wine /media/win/X68000/run68/run68.exe
HAS=$(RUN68) ../x/has060.x
LINK=$(RUN68) ../x/xc/BIN/LK.X
ICONV=iconv -f Shift_JIS -t utf8
TRANS=php ~/trans.php

all: \
    ansi.x \
    datetime.x \
    filewrite.x \
    fwritetsr.x \
    ioctrl.x \
    oki1.x \
    oki2.x \
    ontime.x \
    opm.x \
    print.x \
    proginfo.x \
    sjis.x \
    super1.x \
    super2.x \
    super3.x \
    ver.x

%.x:
	$(LINK) $^ -o $@
	stty sane

ansi.x: ansi.o
datetime.x: datetime.o
filewrite.x: filewrite.o
fwritetsr.x: fwritetsr.o
ioctrl.x: ioctrl.o
nameck.x: nameck.o
ontime.x: ontime.o
oki1.x: oki1.o
oki2.x: oki2.o
opm.x: opm.o
print.x: print.o
proginfo.x: proginfo.o
sjis.x: sjis.o
super1.x: super1.o
super2.x: super2.o
super3.x: super3.o
ver.x: ver.o

%.o: %.s
	$(HAS) -i . $< 2>&1 | $(ICONV)
	stty sane

clean:
	rm -f *.o *.x
