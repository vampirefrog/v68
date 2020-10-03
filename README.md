v68
===

A Human68k X68000 CUI emulator. Very similar to [run68](https://github.com/rururutan/run68), it is written from scratch and emulates, besides the OS calls and the CPU, the sound hardware.

Uses [Musashi](https://github.com/kstenerud/Musashi) for CPU emulation.

In the [docs](docs) folder, there are some partially translated versions of [puni](https://nfggames.com/X68000/index.php/Mirrors/Groundzero%20Organization/x68tools/develop/docs/puni/) docs.

Building on debian 10
---------------------

First, have some required packages ready:

```
sudo apt-get install git build-essential portaudio19-dev zlib1g-dev
# if we don't touch these, you'll need wine to build them with run68.exe
touch fake_ipl.inc fake_human.inc
make
```

Building the tests:

```
sudo dpkg --add-architecture i386 && sudo apt-get update && sudo apt-get install wine32
```
