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
	struct protocol_interface	*pr_cb;
	struct hw_interface		*hw_cb;

	enum {
		SBP_STATE_INIT,		/**< the device is initialising and waiting to receive its first valid packet */
		SBP_STATE_IDLE,		/**< the device is idling - data can be sent at this stage */
		SBP_STATE_XMIT,		/**< a frame is being transmitted */
		SBP_STATE_RECV,		/**< a frame is being received */
		SBP_STATE_IGNORE	/**< a frame is being ignored */
	} state;
} sblp_data;

void sblp(struct hw_interface *hw_cb, struct protocol_interface *pr_cb) {
	/* store local copies of the HW structs */
	sblp_data.hw_cb = hw_cb;
	sblp_data.pr_cb = pr_cb;

	sblp_data.state = SBP_STATE_INIT;
}
