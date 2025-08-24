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
.globl spin
spin:	jra  spin

# This ends up 2 words
#  (as a PC relative jump interestingly)
#  even though a short bra would do.
spin2:	jmp	 spin2

/*
loop:	bra	loop
*/

.globl get_crp
get_crp:
		moveal %sp@(4), %a0
		pmove %crp, %a0@
		rts

.globl get_srp
get_srp:
		moveal %sp@(4), %a0
		pmove %srp, %a0@
		rts

.globl get_tc
get_tc:
		moveal %sp@(4), %a0
		pmove %tc, %a0@
		rts

.globl get_psr
get_psr:
		moveal %sp@(4), %a0
		pmove %psr, %a0@
		rts

.globl get_sr
get_sr:
		moveq #0, %d0
		movew %sr, %d0
		rts

# gets Exception 0x2C at 0x00004040
# Using d0 is NOT allowed for pmove
.globl get_tc_X
get_tc_X:
		pmove %tc, %d0
		rts

# Idiotic junk to avoid complaints from linker
.section        .note.GNU-stack,"",%progbits

/* THE END */
