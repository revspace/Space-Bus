/** \file sblp.c
 * \brief Implements the hardware-independent part of the SpaceBus Link Protocol.
 *
 * This is, once again, implemented as a state machine. The protocol
 * starts by using the HW layer to fish for its first valid sync
 * sequence, then receives the message indicated by it and starts idling.
 * 
 * \todo many things, needs more implementation
 */

#include <avr/io.h>
#include "../interop.h"

#define SBLP_BUFSIZE	50
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
	
	uint8_t		 recv_payload[SBLP_BUFSIZE];
	uint8_t		*xmit_payload;
	uint16_t	 index;
} sblp_data;

void sblp_init() {
	sblp_data.state = SBLP_STATE_IDLE;
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
					sblp_data.header.length |= b;
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

		case SBLP_STATE_RECV_PAYLOAD:
			sblp_data.recv_payload[sblp_data.index++] = b;
			if(sblp_data.index > sblp_data.header.length) {
				sblp_data.state = SBLP_STATE_IDLE;

				frame_received(&sblp_data.header, sblp_data.recv_payload);
			}
			break;

		case SBLP_STATE_IGNORE:
			/* count down the bytes until we're done */
			if(++sblp_data.index > sblp_data.header.length)
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
				case 0:	/* sync */
					send_sync();
					sblp_data.index++;
					break;

				case 1:	/* type */
					send_byte(sblp_data.header.type);
					sblp_data.index++;
					break;

				case 2:	/* length MSB */
					send_byte((sblp_data.header.length >> 8) & 0xFF);
					sblp_data.index++;
					break;

				case 3:	/* length LSB */
					send_byte(sblp_data.header.length & 0xFF);
					sblp_data.index++;
					break;

				case 4:	/* destination address */
					send_byte(sblp_data.header.dest);
					sblp_data.index++;
					break;

				case 5:	/* source address */
					send_byte(sblp_data.header.src);

					/* done sending header -- start sending payload */
					sblp_data.index = 0;
					sblp_data.state = SBLP_STATE_XMIT_PAYLOAD;
					break;
			} ;
			break;			

		case SBLP_STATE_XMIT_PAYLOAD:
			send_byte(sblp_data.xmit_payload[sblp_data.index++]);
			if(sblp_data.index > sblp_data.header.length) {
				frame_sent();
				sblp_data.state = SBLP_STATE_IDLE;
			}

			end_transmission();
			break;

		default:
			/* shouldn't happen -- ignore */
			break;
	}
}


/* functions called by layer above */
uint8_t send_frame(struct sblp_header *header, uint8_t *payload) {
	if(sblp_data.state == SBLP_STATE_IDLE) {
		/* fill in header and send frame */
		sblp_data.header.type	= header->type;
		sblp_data.header.length = header->length;
		sblp_data.header.src	= header->src;
		sblp_data.header.dest	= header->dest;

		sblp_data.xmit_payload = payload;
		sblp_data.index = 1;

		begin_transmission();
		sblp_data.state = SBLP_STATE_XMIT_HEADER;

		send_sync();
		return 1;
	} else {
		/* can't send frame when receiving, syncing etc. */
		return 0;
	}
}
