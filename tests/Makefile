ifeq ($(OS),Windows_NT)
RUN68=../run68/run68.exe
else
RUN68=wine ../run68/run68.exe
endif

HAS=$(RUN68) ../x/HAS.X -w3
LINK=$(RUN68) ../x/LK.X
ICONV=../x68ksjis/sjis2utf8
TRANS=php ~/trans.php

all: \
	ansi.x \
	b_memstr.x \
	datetime.x \
	filewrite.x \
	fwritetsr.x \
	idiv.x \
	ioctrl.x \
	iusing.x \
	nameck.x \
	oki1.x \
	oki2.x \
	oki3.x \
	oki4.x \
	ontime.x \
	opm.x \
	pcm8_1.x \
	print.x \
	process.x \
	proginfo.x \
	sjis.x \
	super1.x \
	super2.x \
	super3.x \
	umod.x \
	ver.x

%.x:
	$(LINK) $(filter %.o,$^) -o $@
	stty sane

ansi.x: ansi.o
b_memstr.x: b_memstr.o
datetime.x: datetime.o
filewrite.x: filewrite.o
fwritetsr.x: fwritetsr.o
idiv.x: idiv.o
ioctrl.x: ioctrl.o
iusing.x: iusing.o
mxdrv17.x: mxdrv17.o
nameck.x: nameck.o
ontime.x: ontime.o
oki1.x: oki1.o
oki2.x: oki2.o
oki3.x: oki3.o
oki4.x: oki4.o tools.o
opm.x: opm.o
pcm8_1.x: pcm8_1.o piano-c4.adp piano-e4.adp piano-g4.adp
print.x: print.o
process.x: process.o
proginfo.x: proginfo.o
sjis.x: sjis.o
super1.x: super1.o
super2.x: super2.o
super3.x: super3.o
umod.x: umod.o
ver.x: ver.o

%.o: %.s
	$(HAS) -i . $< 2>&1 | $(ICONV)
	stty sane

%.adp: %.wav
	@#sox $< -c 1 -r 15625 -t ima -e ima-adpcm $@
	sox $< -c 1 -r 15625 -t vox -e oki-adpcm $@

clean:
	rm -f *.o *.x *.adp
