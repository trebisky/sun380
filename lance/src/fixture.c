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

#include "../h/pixrect.h"
#include "../h/protos.h"

/* This is defined in h/bootparam.h -- BUT
 * this value won't do for us, so we pick
 * something different.
 */
/* Load address of boot pgms */
// #define LOADADDR    0x4000
#define LOADADDR       0x40000

/* XXX
 * etherstrategy is in (of all places) tftp.c 
 */

/* ***************************************** */
/* Routines that the driver makes calls to.
 */

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

        if (devopen(&req))      /* Do all the hard work */
                return -1;

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
