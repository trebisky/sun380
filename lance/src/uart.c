/* serial.c
 *
 * uart driver for the SCC on the sun3/280
 *  This is the Zilog Z8530
 *  5-20-2025 -- Tom Trebisky
 */
#include "protos.h"

typedef volatile unsigned short vu_short;
typedef volatile unsigned char vu_char;

#define BIT(nr)		(1<<(nr))

/* Write here to send data to serial A
 * 0x0fe02006
 * port A really comes second the way Sun wired it all up.
 * My eeprom location 0x1f is set to 0x10 for "use port A"
 * and I am getting data on port A as labeled on the machine.
*/

struct scc {
		vu_char		csr_b;		/* 00 */
		char		_pad1;
		vu_char		data_b;		/* 02 */
		char		_pad2;
		vu_char		csr_a;		/* 04 */
		char		_pad3;
		vu_char		data_a;		/* 06 */
};

// #define SCC_BASE 0x0fe02000
#define SCC_BASE    0xfef02000


#define ST_TXE	BIT(2)
#define ST_RXRDY	BIT(0)

void
uart_init ( void )
{
	/* nothing here, we rely on the boot roms */
}

void
uart_stat ( void )
{
	struct scc *sp = (struct scc *) SCC_BASE;

	printf ( "Uart status: %h\n", sp->csr_a );
}

void
uart_putc ( int cc )
{
		struct scc *sp = (struct scc *) SCC_BASE;

		if ( cc == '\n' )
			uart_putc ( '\r' );

		while ( ! (sp->csr_a & ST_TXE ) )
			;
		sp->data_a = cc;
}

int
uart_getc ( void )
{
		struct scc *sp = (struct scc *) SCC_BASE;
		int rv;

		while ( ! (sp->csr_a & ST_RXRDY ) )
			;

		rv = sp->data_a & 0x7f;

		if ( rv == '\r' )
			rv = '\n';

		/* Things are nicer when we echo */
		uart_putc ( rv );

		return rv;
}

/* Portable code below here */

void
uart_gets ( char *buf )
{
	char *p = buf;
	int c;

	for ( ;; ) {
	    c = uart_getc ();
	    if ( c == '\n' ) {
			*p = '\0';
			return;
	    }
	    *p++ = c;
	}
}


void
uart_puts ( char *s )
{
        while ( *s ) {
            if (*s == '\n')
                uart_putc('\r');
            uart_putc(*s++);
        }
}

#ifdef notdef
void
puts ( char *msg )
{
		while ( *msg )
			putch ( *msg++ );
}
#endif

/* THE END */
