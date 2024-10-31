/* xstrings.c -- pull strings out of our ROM image
 * Tom Trebisky  10-30-2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define MAX_ROMSIZE 150000
#define BIN_PATH "sun380.bin"

char buf[MAX_ROMSIZE];
int romsize;

unsigned int rombase = 0xfefe0000;
unsigned int romend;

#define STR_START	0xfeff3c8c
#define STR_END 	0xfeff4266

void
read_rom ( char *path )
{
	int rom;

	rom = open ( path, O_RDONLY );
	if ( rom < 0 ) {
	    fprintf ( stderr, "Cannot access file: %s\n", path );
	    exit ( 1 );
	}

	romsize = read ( rom, buf, MAX_ROMSIZE );
	printf ( " ... %d bytes read\n", romsize );
	close ( rom );

	romend = rombase + romsize - 1;
}

int
dump_string ( char *ss, int index )
{
		int newl;
		char *p;
		int c;

		newl = 0;
		p = ss;
		while ( c = buf[index] ) {
			if ( c == '\n' ) {
				*p++ = '\\';
				*p++ = 'n';
				newl = 1;
			} else if ( c < ' ' || c > '~' ) {
				*p++ = '.';
			} else
				*p++ = c;
			index++;
			if ( newl && buf[index] ) {
				*p++ = ' ';
				*p++ = '+';
				*p++ = '+';
				break;
			}
		}
		*p = '\0';

		return index;
}

void
batch ( unsigned int start, unsigned int end )
{
	int o_start, o_end;
	int index;
	unsigned int a_str;
	char ss[1024];

	o_start = start - rombase;
	o_end = end - rombase;
	index = o_start;

	for ( ;; ) {
		a_str = rombase + index;
		index = dump_string ( ss, index );
		printf ( "%08x: %s\n", a_str, ss );

		while ( buf[index] == '\0' )
			index++;
		if ( index >= o_end )
			break;
	}
}

void
sorry ( void )
{
		fprintf ( stderr, "Sorry\n" );
		exit ( 1 );
}

void
single ( char *arg_addr )
{
		unsigned int addr;
		char ss[1024];
		int index;

		addr = strtol ( arg_addr, NULL, 16 );
		if ( addr < rombase )
			sorry ();
		if ( addr > romend )
			sorry ();

		index = addr - rombase;
		(void) dump_string ( ss, index );
		// printf ( "%08x: %s\n", addr, ss );
		printf ( "%s\n", ss );
}

int
main ( int argc, char **argv )
{
		read_rom ( BIN_PATH );

		argc--;
		argv++;

		if ( argc == 0 ) {
			// batch ( STR_START, STR_END );
			batch ( 0xfeff4468, 0xfeff4c90 );
			printf ( "DONE\n" );
		} else
			single ( argv[0] );

		return 0;
}

/* THE END */
