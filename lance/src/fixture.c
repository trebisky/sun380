/* fixture.c
 *
 * This is the bootrom "emulator" that
 * does the least possible work to supply
 * an environment that allows the ethernet
 * driver to run and be tested.
 *
 *  Tom Trebisky  8-27-2025
 */

#include "../h/sunromvec.h"
#include "../h/idprom.h"
#include "../h/cpu.map.h"
#include "../dev/saio.h"
#include "../h/socket.h"
#include "../h/in.h"
#include "../h/if.h"
#include "../h/if_ether.h"

#include "../h/pixrect.h"
#include "../h/protos.h"

/* This is defined in h/bootparam.h -- BUT
 * this value won't do for us, so we pick
 * something different.
 */
/* Load address of boot pgms */
// #define LOADADDR    0x4000
#define LOADADDR       0x40000

/* Currently this test resides at 0x4000 to 0x5f66
 * Surprisingly, TFTP just grabs memory at 0x3000,
 * which is OK I guess.
 */

/* This is from sun3/cpu.addrs.h
 * I find this interesting.  The bootrom seems to
 * call devalloc() to set up a new mapping.  Maybe.
 * That call is conditional on d_devbytes, which never
 * seems to get set.  Whatever the case, I see no reason
 * to call devalloc() to get the base address given that
 * a mapping is already set up.
 */
// #define AMD_ETHER_BASE  ((struct amd_ether *)        0x0FE10000)
// #define AMD_ETHER_BASE  ((struct amd_ether *)           0xFEF0E000)
#define AMD_ETHER_BASE  0xFEF0E000;

/* XXX
 * etherstrategy is in (of all places) tftp.c 
 */

/* ***************************************** */
/* Set things up and call the boot code
 */

extern struct boottab ledriver;

static struct bootparam boot_info;

int millitime ( void );

void
lance_test ( void )
{
		int rv;
		struct bootparam *bp;

		bp = &boot_info;
        bp->bp_boottab = &ledriver;

		printf ( "Milli = %d\n", millitime () );
		printf ( "Calling tftpboot\n" );
		printf ( "Milli = %d\n", millitime () );
		/* We could call this using the pointer
		 * in the boottab, i.e. via ledriver.boot
		 * But in our case, this makes things clear.
		 */
		rv = tftpboot ( bp );
		printf ( "tftpboot returns %X\n", rv );
}

/* ***************************************** */
/* Routines that the driver makes calls to.
 */

// struct ether_addr etherbroadcastaddr = { 
//         0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,     
// };

struct ether_addr myether = { 
        0x08, 0, 0x20, 1, 0xdb, 2     
};

// moved here from inet.c
void
myetheraddr ( struct ether_addr *ea )
{
        *ea = myether;
}

// tjt - this is correct for the 3/160
// #define	romp	((struct sunromvec *)0x0FEF0000)
// #define millitime() (*romp->v_nmiclock)
#define	ROMBASE	((struct sunromvec *) 0xFEFE0000)

// tftp.c uses this now
int
millitime ( void )
{
		return *ROMBASE->v_nmiclock;
}

// tjt - my replacement for devopen()
// The saioreq struct is on the stack from
// tftpboot
void
io_setup ( struct saioreq *sip )
{
		sip->si_devaddr = (char *) AMD_ETHER_BASE;

		/* XXX */
		sip->si_dmaaddr = (char *) 0x00700000;

		/* calls lanceopen() */
		(sip->si_boottab->b_open) (sip);
}

#ifdef notdef
/* From sys/xxboot.c
 */
int
devopen ( struct saioreq *sip )
{
        register struct devinfo *dp;
        char *a;

        sip->si_devaddr = sip->si_devdata = sip->si_dmaaddr = (char *)0;
        dp = sip->si_boottab->b_devinfo;
        if (dp) {
                /* Map controller number into controller address */
                if (sip->si_ctlr < dp->d_stdcount) {
                        sip->si_ctlr = (int)((dp->d_stdaddrs)[sip->si_ctlr]);
                }
                /* Map in device itself */
                if (dp->d_devbytes) {
                        a = devalloc ( dp->d_devtype, (char *) sip->si_ctlr,
                                dp->d_devbytes);
                        if (!a)
                                return (-1);
                        sip->si_devaddr = a;
                }
                if (dp->d_dmabytes) {
                        a = resalloc(RES_DMAMEM, dp->d_dmabytes);
                        if (!a)
                                return (-1);
                        sip->si_dmaaddr = a;
                }
                if (dp->d_localbytes) {
                        a = resalloc(RES_MAINMEM, dp->d_localbytes);
                        if (!a)
                                return (-1);
                        sip->si_devdata = a;
                }
        }

        return ((sip->si_boottab->b_open)(sip));
}
#endif

/* also from sys/xxboot.c
 */
int
tftpboot ( struct bootparam *bp )
{
        struct saioreq req;
        int blkno;
        char *addr;

        req.si_ctlr = bp->bp_ctlr;
        req.si_unit = bp->bp_unit;
        req.si_boff = (daddr_t)bp->bp_part;
        req.si_boottab = bp->bp_boottab;

		io_setup ( &req );
#ifdef notdef
        if (devopen(&req))      /* Do all the hard work */
                return -1;
#endif

		printf ( "Calling tftpload\n" );
        if (tftpload(&req) == -1){
                return(-1);
        } else {
                return LOADADDR;
        }
}


/* From sys/idprom.c
 * Used by the probe routine
 */
int
idprom ( unsigned char format, void *aid )
// idprom ( unsigned char format, struct idprom *id )
{
	struct idprom *id = (struct idprom *) aid;

	id->id_machine = IDM_SUN3_M25;
	return IDFORM_1;
}

/* Verbatim from sys/common.c
 */
void
bzero ( char *p, int n )
{
        register char zeero = 0;

        while (n > 0)
                *p++ = zeero, n--;      /* Avoid clr for 68000, still... */
}

void
bcopy ( char *src, char *dest, int count )
{
        count--;
        do {
                *dest++ = *src++;
        } while (--count != -1);
}

/* The bootrom calls code in sun3/cmp.s
 * This is a sloppy version, but good enough for inet.c
 */
int
bcmp ( caddr_t a, caddr_t b, int size )
{
	while ( size ) {
		if ( *a != *b )
			return size;
		size--;
		a++;
		b++;
	}
	return 0;
}

/* From sys/printf.c
 */
static char chardigs[]="0123456789ABCDEF";

void uart_putc ( int );

/*
 * printhex() prints rightmost <digs> hex digits of <val>
 */
void
printhex ( int val, int digs )
{
        digs = ((digs-1)&7)<<2;         /* digs == 0 => print 8 digits */
        for (; digs >= 0; digs-=4)
			uart_putc ( chardigs[(val>>digs)&0xF] );
			// putchar(chardigs[(val>>digs)&0xF]);
}

/* THE END */
