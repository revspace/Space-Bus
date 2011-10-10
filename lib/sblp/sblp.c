/** \file sblp.c
 * \brief Implements the hardware-independent part of the SpaceBus Link Protocol.
 *
 * This is, once again, implemented as a state machine. The protocol
 * starts by using the HW layer to fish for its first valid sync
 * sequence, then receives the message indicated by it and starts idling.
 * 
 * \todo many things, needs more implementation
 */

#include "../interop.h"

/** internal data for the protocol stack */
struct {
	enum {
		SBLP_STATE_INIT,		/**< the device is initialising and waiting to receive its first valid packet */
		SBLP_STATE_IDLE,		/**< the device is idling - data can be sent at this stage */
		SBLP_STATE_XMIT,		/**< a frame is being transmitted */
		SBLP_STATE_RECV_HEADER,		/**< a frame is being received -- we're in the header */
		SBLP_STATE_RECV_PAYLOAD,	/**< a frame is being received -- we're in the payload */
		SBLP_STATE_IGNORE		/**< a frame is being ignored */
	} state;

	uint16_t length;
	uint16_t index;
} sblp_data;

void sblp_init() {
	sblp_data.state = SBLP_STATE_INIT;
}

/* functions called by layer below */
void sync_received() {
	switch(sblp_data.state) {
		case SBLP_STATE_INIT:
			/* sync received -- we are in business! */
			sblp_data = SBLP_STATE_RECV_HEADER;
			break;
		
		case SBLP_STATE_IDLE:
			/* sync indicates start of a new message */
			sblp_data = SBLP_STATE_RECV_HEADER;
			break;
			
		default:
			/* shouldn't happen -- ignore */
			break;
	}
}

void byte_received(uint8_t b) {
	switch(sblp_data.state) {
		default:
			/* shouldn't happen -- ignore */
			break;
	}
}

void byte_sent() {
	
}

void sync_received() {
	
}

/* functions called by layer above */
uint8_t send_frame(uint8_t *frame) {
	if(sblp_data.state == SBLP_STATE_IDLE) {
		/* send a frame */
		sblp_data.state == SBLP_STATE_XMIT;
		return 1;
	} else {
		/* can't send frame when receiving, syncing etc. */
		return 0;
	}
}
