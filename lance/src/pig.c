/* pig.c
 */
#include "protos.h"

volatile unsigned char io;

// send ( int x )
// send ( char x )
static void
send ( int x )
{
		io = x;
}

void
pig ( char *bb )
{
	// char *bb = "abcdefgh";
	char *p;

	for ( p=bb; *p; p++ )
		send ( *p );
}


/* THE END */
