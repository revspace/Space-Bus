/** \file sblp.c
 * \brief Implements the hardware-independent part of the SpaceBus Link Protocol.
 *
 * This is, once again, implemented as a state machine. The protocol
 * starts by using the HW layer to fish for its first valid sync
 * sequence, then receives the message indicated by it and starts idling.
 */

#include "interop.h"
#include "sblp.h"

/** internal data for the protocol stack */
static struct {
	struct protocol_interface	*pr_if;
	struct hw_interface		*hw_if;

	enum {
		SBP_STATE_INIT,		/**< the device is initialising and waiting to receive its first valid packet */
		SBP_STATE_IDLE,		/**< the device is idling - data can be sent at this stage */
		SBP_STATE_XMIT,		/**< a frame is being transmitted */
		SBP_STATE_RECV,		/**< a frame is being received */
		SBP_STATE_IGNORE	/**< a frame is being ignored */
	} state;
} sblp_data;

void sblp_byte_received(uint8_t b,void *d);
void sblp_byte_sent(void *d);
void sblp_sync_received(void *d);

void sblp_send_frame(struct frame f);
void sblp_recv_frame();

void sblp(struct hw_interface *hw_if, struct protocol_interface *pr_if) {
	/* store local copies of the interface structs */
	sblp_data.hw_if = hw_if;
	sblp_data.pr_if = pr_if;

	/* fill structs with function pointers */
	sblp_data.hw_if->u_byte_received	= &sblp_byte_received;
	sblp_data.hw_if->u_byte_sent		= &sblp_byte_sent;
	sblp_data.hw_if->u_sync_received	= &sblp_sync_received;

	sblp_data.pr_if->d_send_frame		= &sblp_send_frame;
	sblp_data.pr_if->d_recv_frame		= &sblp_recv_frame;

	sblp_data.state = SBP_STATE_INIT;
}

/* functions called by layer below */
void sblp_byte_received(uint8_t b,void *d) {
	
}

void sblp_byte_sent(void *d) {
	
}

void sblp_sync_received(void *d) {
	
}

/* functions called by layer above */
void sblp_send_frame(struct frame frame) {
	
}

void sblp_recv_frame() {
	
}
