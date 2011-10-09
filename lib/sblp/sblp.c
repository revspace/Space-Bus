/** \file sblp.c
 * \brief Implements the hardware-independent part of the SpaceBus Link Protocol.
 *
 * This is, once again, implemented as a state machine. The protocol
 * starts by using the HW layer to fish for its first valid sync
 * sequence, then receives the message indicated by it and starts idling.
 */

#include "../interop.h"
#include "sblp.h"

/** internal data for the protocol stack */
static enum {
	SBP_STATE_INIT,		/**< the device is initialising and waiting to receive its first valid packet */
	SBP_STATE_IDLE,		/**< the device is idling - data can be sent at this stage */
	SBP_STATE_XMIT,		/**< a frame is being transmitted */
	SBP_STATE_RECV,		/**< a frame is being received */
	SBP_STATE_IGNORE	/**< a frame is being ignored */
} sblp_state;

void byte_received(uint8_t b,void *d);
void byte_sent(void *d);
void sync_received(void *d);

void send_frame(struct frame f);
void recv_frame();

void sblp_state() {
	sblp_state = SBP_STATE_INIT;
}

/* functions called by layer below */
void byte_received(uint8_t b,void *d) {
	
}

void byte_sent(void *d) {
	
}

void sync_received(void *d) {
	
}

/* functions called by layer above */
void send_frame(struct frame frame) {
	
}

void recv_frame() {
	
}
