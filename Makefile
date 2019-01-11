MUSASHIFILES=m68kcpu.c m68kdasm.c
MUSASHIGENCFILES=m68kops.c m68kopac.c m68kopdm.c m68kopnz.c

MUSASHIOBJS=$(patsubst %.c,musashi/%.o,$(MUSASHIFILES))
MUSASHIGENOBJS=$(patsubst %.c,musashi/%.o,$(MUSASHIGENCFILES))

all: v68 xinfo sjis2utf8

$(patsubst %.c,musashi/%.c,$(MUSASHIGENCFILES)): musashi/m68kmake musashi/m68k_in.c
	(cd musashi && ./m68kmake)

musashi/m68kmake: musashi/m68kmake.o
	gcc $^ -o $@

v68: main.o tools.o v68.o v68ipl.o v68io.o v68periph.o v68human.o v68doscall.o v68iocscall.o v68fecall.o sjis.o sjis_unicode.o utf8.o ym2151.o dmac.o okim6258.o vgm.o $(MUSASHIOBJS) $(MUSASHIGENOBJS)
	gcc $^ $(shell pkg-config --libs ao)  -lm -o $@

xinfo: xinfo.o md5.o cmdline.o
	gcc $^ -o $@

sjis2utf8: sjis2utf8.o sjis.o sjis_unicode.o utf8.o
	gcc $^ -o $@

test-mem: test-mem.o v68.o v68human.o v68opm.o v68io.o v68doscall.o v68fecall.o v68iocscall.o okim6258.o ym2151.o vgm.o sjis.o sjis_unicode.o $(MUSASHIOBJS) $(MUSASHIGENOBJS)
	gcc $^ -lao -lm -o $@

%.o: %.c
	gcc -g -Wall $(shell pkg-config --cflags ao) -c $< -o $@

clean:
	rm -f v68 xinfo sjis2utf8 *.o musashi/*.o $(patsubst %.c,musashi/%.c,$(MUSASHIGENCFILES)) ay.js *.wasm *.map test

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
