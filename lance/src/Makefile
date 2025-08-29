# Build a bare metal serial demo for the Sun3/280
#
# Tom Trebisky 5/23/2025
#
# This is from a Fedora package
#CC = m68k-linux-gnu-gcc -m68020 -O2 -fno-builtin
CC = m68k-linux-gnu-gcc -m68020 -fno-builtin -Wno-endif-labels

#AS = m68k-linux-gnu-as -m68020
AS = m68k-linux-gnu-as -m68020 --register-prefix-optional
LD = m68k-linux-gnu-ld --no-warn-rwx-segments
DUMP = m68k-linux-gnu-objdump -d

OBJS = start.o pig.o main.o prf.o uart.o util.o fixture.o if_le.o tftp.o inet.o

# Note that the path for libgcc will change with new gcc
# releases, which would require this Makefile to be updated
# whenever a new release came out.
# There is a way around this, but I now avoid this entirely
#LOPT = -T sun3.lds -L /usr/lib/gcc/m68k-linux-gnu/11
LOPT = -T sun3.lds -L .

# My prf.c requires these routines,which are in -lgcc
# undefined reference to `__modsi3'
# undefined reference to `__divsi3'
# Currently in: /usr/lib/gcc/m68k-linux-gnu/11/libgcc.a
LIBS = -lm68k

.c.o:
	$(CC) $< -c

.S.o:
	$(AS) $< -o $@

#	$(AS) -o start.o start.S

all: fancy.bin fancy.dump

install: fancy.bin
	cp fancy.bin /var/lib/tftpboot/sun3x_test.bin

getlib:
	cp ../libgcc/libm68k.a .

# As of 3-1-2022, I am using gcc 11.2.1
version:
	$(CC) --version

# Here is the crucial part, linking.
#  the -s switch says to strip the symbol table.
#  note the handy binary image output option!
fancy.bin:	$(OBJS)
	$(LD) $(LOPT) --oformat binary -s -o fancy.bin $(OBJS) $(LIBS)

fancy.elf:	$(OBJS)
	## $(LD) -T sun3.lds -s -o fancy.elf $(OBJS) $(LIBS)
	$(LD) -T sun3.lds -L . -o fancy.elf $(OBJS) $(LIBS)

fancy.dump:	fancy.elf
	$(DUMP) fancy.elf >fancy.dump

ROM = /Projects/sun3-prom/Github
get:
	cp $(ROM)/dev/if_le.c .
	cp $(ROM)/dev/if_lereg.h .
	cp $(ROM)/sys/tftp.c .
	cp $(ROM)/sys/inet.c .

clean:
	rm -f $(OBJS) fancy.bin 
	rm -f *.o *.dump
	rm -f fancy.elf fancy.s

# THE END
