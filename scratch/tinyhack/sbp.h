/* sbp.h - space bus protocol library header
 * This (slightly hacky) implementation of the SBP library is written
 * for the ATTiny85 and requires access to:
 *
 *	- the USI input pin (DI) and associated pin change interrupt (PCINT0)
 * 	- the USI output pin
 * 	- a GPIO pin called DEN (data enable)
 * 	- timer 0
 *
 * General TODO:
 * 	- separate SBP lib from hardware layer
 *	- fix issues outlined in sbp.c
 * 	- probably a better interface
 */

#ifndef _SBP_H

#include <inttypes.h>

/* user-supplied data */
#define SB_DIN	 PB0	/* the USI data in pin */
#define SB_DO	 PB1	/* the USI data out pin */
#define SB_DEN	 PB4	/* the GPIO data enable pin */

#define SB_PORT	PORTB	/* the port these pins are on */
#define SB_DDR	 DDRB	/* DDR for same */

/* as per specification */
typedef struct {
	/* sync is not present because it is
	 * detected/generated in different places.
	 */

	uint8_t      type;
	uint16_t   length;
	uint8_t       dst;
	uint8_t       src;

	uint8_t  *payload;

	uint8_t  checksum;
} sbp_frame_t;

/* initialise space bus lib. */
void		sbp_init(uint8_t address);

/* are we ready to send a new message? */
uint8_t	sbp_idle_get();

/* did we experience an error before? */
uint8_t	sbp_error_get();

/* clear error flag */
void		sbp_error_clear();

/* send a message. buffer must remain valid until sbp_idle() returns true. */
void sbp_send(uint8_t dest, uint16_t size, const uint8_t *msg);

#define _SBP_H
#endif
