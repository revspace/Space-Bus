/** \file tiny485.h
 * Header file for the ATTiny85 hardware layer.
 */

#ifndef _TINY485_H

#include "../sbp/sbp.h"

/* for simplicity's sake, config is also included here */
#define USI_PORT	PORTB	/**< the port the USI in/output pins are on */
#define DEN_PORT	PORTB	/**< the port the data enable pin is on */

#define USI_DDR		DDRB
#define DEN_DDR		DDRB

#define DI		PB0	/**< the DI pin on the USI_PORT */
#define DO		PB1
#define DEN		PB2	/**< the data enable pin on the DEN_PORT */

/** Initialise hardware layer and fill in callbacks.
 *
 * This is the only function the outside world needs to know about -
 * it fills in the callbacks needed to provide an interface to the SBLP
 * (or other, for that matter) layer, then initialises itself. After
 * this, the callbacks specified in the struct hw_callbacks are valid
 * and can be called for further interaction with this layer.
 */
void tiny485(struct hw_callbacks *cb);

#define _TINY485_C
#endif
