/* romput.c
 * 8-22-2025
 */

#ifdef notdef
/* Here is the start of "romvec" from sun3/romvec.s
 */
 .long  INITSP          | Initial SSP for hardware RESET
 .long  _hardreset      | Initial PC  for hardware RESET
 .long  _diag_berr      | Bus error handler for diagnostics
 .long  bootaddr        | Addr of addr of boot parameters
 .long  g_memorysize    | Physical onboard memory size
 .long  getchar         | Get char from cur input source
 .long  putchar         | Put char to current output sink
 .long  mayget          | Maybe get char from current input source
 .long  mayput          | Maybe put char to current output sink

/* And here is ROM disassembly for
 * that table at start of ROM.
 */

fefe0000 fef6 0c00   ; 0 - initial stack
fefe0004 fefe 0228   ; 1 - start address (first PC)

fefe0008 fefe 073a   ; 2 - bus error ("terminate")
fefe000c fefe 00f8   ; 3 - address error (problem)
fefe0010 fef7 21d6   ; 4
fefe0014 fefe 1f50   ; 5 - pickle
fefe0018 fefe 1e74   ; 6 - putchar
fefe001c fefe 1f80   ; 7 - relish
fefe0020 fefe 1eaa   ; 8 - putchar2
#endif

struct romvec {
		long initsp;
		long hardreset;
		void *berr;
		long bootaddr;
		long memsize;
		void *getchar;
		void *putchar;
		void *mayget;
		void *mayput;
};

typedef void (*vfptr) ( int );
typedef int (*ifptr) ( int );

#define ROMBASE	0xfefe0000

void
putchar_rom ( int c )
{
		struct romvec *rvp = (struct romvec *) ROMBASE;
		// ifptr fp = (vfptr) rvp->putchar;
		ifptr fp = (ifptr) rvp->mayput;

		for ( ;; ) {
			if ( (*fp) ( c ) == 0 )
				break;
		}
}

void
romvec_puts ( char *s )
{
        while ( *s ) {
            if (*s == '\n')
                putchar_rom('\r');
            putchar_rom(*s++);
        }
}

/* THE END */
