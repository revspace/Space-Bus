/*	sb.h - spacebus library header
	This implementation of the SBP library requires access to:
		- the USI input pin (DI) and associated pin change interrupt
		- the USI output pin
		- a GPIO pin called DEN (data enable)
		- timer 0
*/

#ifndef _SPACEBUS_H

/* user-supplied data */
#define DIN	 PB0	/* the USI data in pin */
#define DO	 PB1	/* the USI data out pin */
#define DEN	 PB4	/* the GPIO data enable pin */
#define PORT_DEN PORTB	/* the port the data enable pin is on */
#define DDR_DEN	 DDRB	/* DDR for same */

/* initialise space bus lib. */
void		sb_init();

/* are we ready to send a new message? */
uint8_t	sb_idle();

/* send a message. buffer must remain valid until sb_idle() returns true. */
void		sb_send(uint16_t size, const uint8_t *msg);

#define _SPACEBUS_H
#endif
