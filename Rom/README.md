Sun 3/80 project - the bootrom

The bootrom is a single chip (nice that the 68030 works that way).
It is a 128K device, an ATMEL 27C010.

1. sun380.bin - the binary raw readout of the ROM
1. rom.dis - my annotated disassembly of the ROM

This is Rom version 3.0.2

What you probably would enjoy looking at is rom.dis, though if you needed
to burn a new ROM for some reason, you will like sun380.bin.

There is 36K of blank space in the ROM (92K is used).
