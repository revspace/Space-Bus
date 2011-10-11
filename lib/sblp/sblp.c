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

#define SBLP_BUFSIZE	50

struct sblp_header {
	uint8_t	type;
	uint16_t	length;
	uint8_t	dest;
	uint8_t	src;
} ;

/** internal data for the protocol stack */
struct {
	enum {
		SBLP_STATE_INIT,		/**< the device is initialising and waiting to receive its first valid packet */
		SBLP_STATE_IDLE,		/**< the device is idling - data can be sent at this stage */
		SBLP_STATE_XMIT_HEADER,		/**< a frame header is being transmitted */
		SBLP_STATE_XMIT_PAYLOAD,	/**< a frame payload is being transmitted */
		SBLP_STATE_RECV_HEADER,		/**< a frame is being received -- we're in the header */
		SBLP_STATE_RECV_PAYLOAD,	/**< a frame is being received -- we're in the payload */
		SBLP_STATE_IGNORE		/**< a frame is being ignored */
	} state;

	struct sblp_header header;
	
	uint8_t	 recv_payload[SBLP_BUFSIZE];
	uint8_t	*xmit_payload;
	uint16_t	 index;
} sblp_data;

void sblp_init() {
	sblp_data.state = SBLP_STATE_INIT;
}

/* functions called by layer below */
void sync_received() {
	switch(sblp_data.state) {
		case SBLP_STATE_INIT:
			/* sync received -- we are in business! */
			sblp_data.state = SBLP_STATE_RECV_HEADER;
			break;
		
		case SBLP_STATE_IDLE:
			/* sync indicates start of a new message */
			sblp_data.state = SBLP_STATE_RECV_HEADER;
			break;
			
		default:
			/* shouldn't happen -- ignore */
			break;
	}
}

void byte_received(uint8_t b) {
	switch(sblp_data.state) {
		case SBLP_STATE_RECV_HEADER:
			switch(sblp_data.index) {
				case 0:		/* sync */
					/* shouldn't occur here -- ignore */
					sblp_data.index++;
					break;
					
				case 1:		/* type */
					sblp_data.header.type = b;
					sblp_data.index++;
					break;
				
				case 2:		/* length MSB */
					sblp_data.header.length = b<<8;
					sblp_data.index++;
					break;
				
				case 3:		/* length LSB */
					sblp_data.header.length += (b-7);
					sblp_data.index++;
					break;
				
				case 4:		/* destination address */
					sblp_data.header.dest = b;
					sblp_data.index++;
					break;
				
				case 5:		/* source address */
					sblp_data.header.src = b;					

					/* end of header -- move to payload */
					sblp_data.state = SBLP_STATE_RECV_PAYLOAD;
					sblp_data.index = 0;
					break;
			}
			break;

		case SBLP_STATE_IGNORE:
			if(sblp_data.header.length-- == 0)
				sblp_data.state = SBLP_STATE_IDLE;
			break;

		default:
			/* shouldn't happen -- ignore */
			break;
	}
}

void byte_sent() {
	switch(sblp_data.state) {
		case SBLP_STATE_XMIT_HEADER:
			switch(sblp_data.index) {
				case 0:
					
					break;

				case 1:
					
					break;

				case 2:
					
					break;

				case 3:
					
					break;

				case 4:
					
					break;

				case 5:
					
					break;
			} ;
			break;

		default:
			/* shouldn't happen -- ignore */
			break;
	}
}


/* functions called by layer above */
uint8_t send_frame(uint8_t *frame, uint16_t length) {
	if(sblp_data.state == SBLP_STATE_IDLE) {
		/* fill in header and send frame */
		sblp_data.header.length = length;

		sblp_data.xmit_payload = frame;
		sblp_data.index = 0;
		sblp_data.state = SBLP_STATE_XMIT_HEADER;

		return 1;
	} else {
		/* can't send frame when receiving, syncing etc. */
		return 0;
	}
}
