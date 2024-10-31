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
}

void
strings ( unsigned int start, unsigned int end )
{
	char ss[1024];
	char *p;
	int o_start, o_end;
	int index;
	int c;
	unsigned int a_str;
	int newl;

	o_start = start - rombase;
	o_end = end - rombase;
	index = o_start;

	for ( ;; ) {
		newl = 0;
		a_str = rombase + index;
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
		printf ( "%08x: %s\n", a_str, ss );
		while ( buf[index] == '\0' )
			index++;
		if ( index >= o_end )
			break;
	}
}

int
main ( int argc, char **argv )
{
		read_rom ( BIN_PATH );
		// strings ( STR_START, STR_END );
		strings ( 0xfeff4468, 0xfeff4c90 );
		return 0;
}

/* THE END */
