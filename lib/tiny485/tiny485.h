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
#define DEN		PB4	/**< the data enable pin on the DEN_PORT */

/** SBLP/HW communication struct
 *
 * This struct contains the callbacks used for communication between the
 * link (SBP) and hardware layers. Callbacks prefixed u_ are up from HW to
 * SBLP, and those marked d_ are down from SBLP to HW. same goes for
 * variables: those marked u_ are allocated/filled by HW and read by
 * SBLP, and vice versa for the i_ ones.
 */
struct hw_callbacks {
	void  *c_data;					/**< custom pointer for link-specific data */
        void (*u_byte_received)(uint8_t b,void *d);	/**< called when the HW layer receives a byte */
        void (*u_byte_sent)(void *d);			/**< called when the HW layer has sent a byte */

        void (*d_begin_transmission)();		/**< called when the link layer wishes to start a (potentially multi-byte) transmission. */
        void (*d_end_transmission)();		/**< called when the link layer wishes to end a (potentially multi-byte) transmission. */
        void (*d_send_byte)(uint8_t b);	/**< called when the link layer wishes to send a single byte (as part of a transmission). */
} ;

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
