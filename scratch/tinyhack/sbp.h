/* sbp.h - space bus protocol library header
 * This implementation of the SBP library is written for the
 * ATTiny85 and requires access to:
 *	- the USI input pin (DI) and associated pin change interrupt
 * 	- the USI output pin
 * 	- a GPIO pin called DEN (data enable)
 * 	- timer 0
 * 
 * General todo:
 * 	- separate SBP lib from hardware layer
 *	- fix issues outlined in sbp.c
 * 	- probably a better interface
 */

#ifndef _SBP_H

#include <inttypes.h>

/* user-supplied data */
#define DIN	 PB0	/* the USI data in pin */
#define DO	 PB1	/* the USI data out pin */
#define DEN	 PB4	/* the GPIO data enable pin */
#define PORT_DEN PORTB	/* the port the data enable pin is on */
#define DDR_DEN	 DDRB	/* DDR for same */

/* initialise space bus lib. */
void		sbp_init(uint8_t address);

/* are we ready to send a new message? */
uint8_t	sbp_idle();

/* send a message. buffer must remain valid until sbp_idle() returns true. */
void		sbp_send(uint16_t size, const uint8_t *msg);

#define _SBP_H
#endif
