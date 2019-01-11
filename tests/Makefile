RUN68=wine ../run68/run68.exe
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
	mxdrv17.x \
	mxp.x \
	nameck.x \
	oki1.x \
	oki2.x \
	oki3.x \
	ontime.x \
	opm.x \
	pcm8.x \
	pcm8_1.x \
	pcm8en.x \
	print.x \
	process.x \
	proginfo.x \
	sjis.x \
	super1.x \
	super2.x \
	super3.x \
	ver.x

%.x:
	$(LINK) $(filter %.o,$^) -o $@
	stty sane

ansi.x: ansi.o
datetime.x: datetime.o
filewrite.x: filewrite.o
fwritetsr.x: fwritetsr.o
ioctrl.x: ioctrl.o
mxdrv17.x: mxdrv17.o
mxp.x: mxp.o
nameck.x: nameck.o
ontime.x: ontime.o
oki1.x: oki1.o
oki2.x: oki2.o
oki3.x: oki3.o
opm.x: opm.o
pcm8.x: pcm8.o
pcm8en.x: pcm8en.o
pcm8_1.x: pcm8_1.o piano-c4.adp piano-e4.adp piano-g4.adp
print.x: print.o
process.x: process.o
proginfo.x: proginfo.o
sjis.x: sjis.o
super1.x: super1.o
super2.x: super2.o
super3.x: super3.o
ver.x: ver.o

%.o: %.s
	$(HAS) -i . $< 2>&1 | $(ICONV)
	stty sane

%.adp: %.wav
	@#sox $< -c 1 -r 15625 -t ima -e ima-adpcm $@
	sox $< -c 1 -r 15625 -t vox -e oki-adpcm $@

clean:
	rm -f *.o *.x *.adp
