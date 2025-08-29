/* main.c
 *
 * From the Callan project "uart" 2022
 *
 * Now for the sun3/280  5-23-2025
 *
 *  5-23-2025Tom Trebisky
 */

#include "protos.h"

typedef volatile unsigned short vu_short;
typedef volatile unsigned char vu_char;

/* Yields an 0.35 second delay */
/* On the Callan */
void
delay_x ( void )
{
	volatile int count;
	
	count = 50000;
	while ( count-- )
	    ;
}

/* This should be about 1 second */
/* On the Callan */
void
delay_one ( void )
{
	volatile unsigned int count;
	
	count = 142857;
	while ( count-- )
	    ;
}

/* On the Callan */
void
delay_ms ( int ms )
{
	volatile unsigned int count;

	count = 143 * ms;
	
	while ( count-- )
	    ;
}

void
bss_clear ( unsigned int *b1, unsigned int *b2 )
{
	while ( b1 < b2 )
	    *b1++ = 0;
}

static char buffer[512];

void
fill_buffer ( int val )
{
	int i;

	for ( i=0; i<512; i++ )
	    buffer[i] = val;
}

int
hextoi ( char *s )
{
        int val = 0;
        int c;

        while ( c = *s++ ) {
            if ( c >= '0' && c <= '9' )
                val = val*16 + (c - '0');
            else if ( c >= 'a' && c <= 'f' )
                val = val*16 + 10 + (c - 'a');
            else if ( c >= 'A' && c <= 'F' )
                val = val*16 + 10 + (c - 'A');
            else
                break;
        }

        return val;
}

int
atoi ( char *s )
{
        int val = 0;

        if ( s[0] == '0' && s[1] == 'x' )
            return hextoi ( &s[2] );

        while ( *s >= '0' && *s <= '9' )
            val = val*10 + (*s++ - '0');

        return val;
}

int
strlen ( char *s )
{
	int rv = 0;

	while ( *s++ )
	    rv++;

	return rv;
}

/* split a string in place.
 * tosses nulls into string, trashing it.
 */
int
split ( char *buf, char **bufp, int max )
{
        int i;
        char *p;

        p = buf;
        for ( i=0; i<max; ) {
            while ( *p && *p == ' ' )
                p++;
            if ( ! *p )
                break;
            bufp[i++] = p;
            while ( *p && *p != ' ' )
                p++;
            if ( ! *p )
                break;
            *p++ = '\0';
        }

        return i;
}

/* A command should look like "r 13 7 7"
 * where the numbers are c h s (cyl, head, sec)
 */
void
run_command ( char *cmd )
{
	char *wp[8];
	int nw;

	printf ( "Command: %s\n", cmd );

	// printf ( "\n" );
	printf ( "Command: %d:: %s\n", strlen(cmd), cmd );
	nw = split ( cmd, wp, 8 );
	printf ( "nw = %d\n", nw );
}

/* The 'q' to quit also needs a newline after it
 */
void
user_test ( void )
{
	char line[64];

	// fill_buffer ( 0xbc );

	for ( ;; ) {
	    uart_puts ( "Ready% " );
	    uart_gets ( line );

	    if ( line[0] == 'q' )
			break;
	    if ( strlen(line) == 0 )
			continue;

	    run_command ( line );
	    // uart_puts ( "Got: " );
	    // uart_puts ( line );
	}
}

void lance_test ( void );

void
start ( void )
{

	uart_init ();

	delay_one ();
	delay_one ();

	printf ( "Starting lance test 8-28-2025\n" );

	// user_test ();
	lance_test ();

	printf ( " -- Lance test done\n" );
}

/* THE END */
