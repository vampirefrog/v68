MUSASHIFILES=m68kcpu.c m68kdasm.c
MUSASHIGENCFILES=m68kops.c m68kopac.c m68kopdm.c m68kopnz.c

MUSASHIOBJS=$(patsubst %.c,musashi/%.o,$(MUSASHIFILES))
MUSASHIGENOBJS=$(patsubst %.c,musashi/%.o,$(MUSASHIGENCFILES))

CC=gcc


all: v68 xinfo x68ksjis/sjis2utf8

x68ksjis/sjis2utf8:
	(cd x68ksjis && make sjis2utf8)

$(patsubst %.c,musashi/%.c,$(MUSASHIGENCFILES)): musashi/m68kmake musashi/m68k_in.c
	(cd musashi && ./m68kmake)

musashi/m68kmake: musashi/m68kmake.o
	gcc $^ -o $@

CFLAGS=-ggdb -Wall -DHAVE_MEMCPY -Ix68ksjis $(shell pkg-config portaudio-2.0 --cflags)
LDFLAGS=-lz $(shell pkg-config portaudio-2.0 --libs)
ifneq (,$(findstring MINGW,$(shell uname -s)))
CFLAGS+=-static-libgcc
LDFLAGS=-liconv -lws2_32 -static-libgcc
else
LDFLAGS=-lm
endif

ifneq (,$(findstring MINGW,$(shell uname -s)))
LDFLAGS+=-lwinmm
endif

v68: main.o tools.o v68.o v68io.o v68human.o v68doscall.o v68iocscall.o v68fecall.o x68ksjis/sjis.o x68ksjis/sjis_unicode.o x68ksjis/utf8.o ym2151.o dmac.o okim6258.o vgm.o v68periph.o v68ipl.o $(MUSASHIOBJS) $(MUSASHIGENOBJS)
	$(CC) $(filter %.o,$^) $(LDFLAGS) -o $@

v68human.o: v68human.c fake_human.inc

v68ipl.o: v68ipl.c fake_ipl.inc

ifeq ($(OS),Windows_NT)
RUN68=run68/run68.exe
else
RUN68=wine run68/run68.exe
endif

HAS060=$(RUN68) ./x/HAS.X
HASFLAGS=-m68000 -w3
LK=$(RUN68) ./x/LK.X

fake_ipl.inc: fake_ipl.s xdump
	include=tests $(HAS060) $(HASFLAGS) $(patsubst %.inc,%.s,$@)
	$(LK) -bff0000 $(patsubst %.inc,%.o,$@)
	./xdump $(patsubst %.inc,%.x,$@) > $@
	rm -f $(patsubst %.inc,%.x,$@) $(patsubst %.inc,%.o,$@)

fake_human.inc: fake_human.s xdump
	include=tests $(HAS060) $(HASFLAGS) $(patsubst %.inc,%.s,$@)
	$(LK) -b006800 $(patsubst %.inc,%.o,$@)
	./xdump $(patsubst %.inc,%.x,$@) > $@
	rm -f $(patsubst %.inc,%.x,$@) $(patsubst %.inc,%.o,$@)

xinfo: xinfo.o md5.o cmdline.o
	$(CC) $^ -o $@

xdump: xdump.o tools.o
	$(CC) $^ -o $@

test-mem: test-mem.o v68.o v68human.o v68opm.o v68io.o v68doscall.o v68fecall.o v68iocscall.o okim6258.o ym2151.o vgm.o x68ksjis/sjis.o x68ksjis/sjis_unicode.o $(MUSASHIOBJS) $(MUSASHIGENOBJS)
	$(CC) $^ -lao -lm -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	cd x68ksjis && make clean
	rm -f v68 v68.exe xinfo xinfo.exe xdump xdump.exe *.o musashi/*.o $(patsubst %.c,musashi/%.c,$(MUSASHIGENCFILES)) ay.js *.wasm *.map test

main.o: main.c v68.h okim6258.h mamedef.h ym2151.h v68io.h cmdline.h \
 tools.h
tools.o: tools.c tools.h
v68.o: v68.c v68.h okim6258.h mamedef.h ym2151.h v68io.h \
 musashi/m68kcpu.h musashi/m68k.h musashi/m68kconf.h
v68io.o: v68io.c v68io.h v68.h okim6258.h mamedef.h ym2151.h
ym2151.o: ym2151.c mamedef.h ym2151.h
okim6258.o: okim6258.c mamedef.h okim6258.h
vgm.o: vgm.c vgm.h
m68kcpu.o: musashi/m68kcpu.c musashi/m68kops.h musashi/m68kcpu.h \
 musashi/m68k.h musashi/m68kconf.h
m68kdasm.o: musashi/m68kdasm.c musashi/m68k.h musashi/m68kconf.h
m68kops.o: musashi/m68kops.c musashi/m68kops.h
m68kopac.o: musashi/m68kopac.c musashi/m68kcpu.h musashi/m68k.h \
 musashi/m68kconf.h
m68kopdm.o: musashi/m68kopdm.c musashi/m68kcpu.h musashi/m68k.h \
 musashi/m68kconf.h
m68kopnz.o: musashi/m68kopnz.c musashi/m68kcpu.h musashi/m68k.h \
 musashi/m68kconf.h
