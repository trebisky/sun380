# disassemble Sun 3/80 boot rom

# I add the -z options because I would
# rather see blocks of zeros verbatim
# than the compression to "..."
DUMP = m68k-linux-gnu-objdump -m68030 -z -d

all:	wrap sun380.dis

wrap: wrap.c
	cc -o wrap wrap.c

xstrings: xstrings.c
	cc -o xstrings xstrings.c

sun380.elf: sun380.bin
	./wrap sun380.bin sun380.elf fefe0000

sun380.dis: sun380.elf
	$(DUMP) sun380.elf >sun380.dis

sun380.odx: sun380.bin
	odx sun380.bin >sun380.odx

fix: sun380.elf
	$(DUMP) --start-address=0xfefe2b04 --stop-address=0xfefe2b10 sun380.elf

strings: xstrings
	./xstrings >zzz
	./xstrings

# ============================================================

callan13.dis: callan13.elf
	$(DUMP) callan13.elf >callan13.tmp
	./hexfix callan13.tmp >callan13.dis
	rm callan13.tmp

clean:
	rm -f wrap
	rm -f *.elf
	rm -f *.dis
	rm -f *.odx
