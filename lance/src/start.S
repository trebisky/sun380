/* Bare metal startup for sun3/280
 *
 * Tom Trebisky  5-23-2025
 *
 */
	.text

# Some random notes about the 3/280 rom
#
# >b?
# Boot syntax: b [!][dev(ctlr,unit,part)] name [options]
# Possible boot devices:
#   id: PANTHER (ipi) controller
#   xd: Xylogics 7053 disk
#   xy: Xylogics 440/450 disk
#   sd: SCSI disk
#   ie: Sun/Intel Ethernet
#   gn: Sun Generic Network
#   xt: Xylogics 472 tape
#   st: SCSI tape

start_here:
	movew #0x2700,%sr

	/* Clear the bss area
	 */
	movel	#__bss_start__, %sp@-
	movel	#__bss_end__, %sp@-
	jsr	bss_clear
	addql	#8, %sp

	bsr start

	rts		| return to bootrom ?

# Write to the DIAG register
#  requires function code fiddling
.globl wr_diag

wr_diag:
	movec %dfc,%a0
	moveq #3,%d1
	movec %d1,%dfc

	moveb %sp@(7),%d0
	movesb %d0,0x70000000
	movec %a0,%dfc
	rts

# jra is an assembler pseudo that lets
#  the assembler use bra or jmp as it
#  finds is best.
spin:	jra  spin

# This ends up 2 words
#  (as a PC relative jump interestingly)
#  even though a short bra would do.
spin2:	jmp	 spin2

		movl    d6,sp@-

/*
loop:	bra	loop
*/

# Idiotic junk to avoid complaints from linker
.section        .note.GNU-stack,"",%progbits

/* THE END */
