#include "interop.h"
#include "sbp.h"

static struct {
	struct protocol_callbacks	*pr_cb;
	struct hardware_callbacks	*hw_cb;

	enum {
		SBP_STATE_INIT,		/**< the device is initialising and waiting to receive its first valid packet */
		SBP_STATE_IDLE,		/**< the device is idling - data can be sent at this stage */
		SBP_STATE_XMIT,		/**< a frame is being transmitted */
		SBP_STATE_RECV,		/**< a frame is being received */
		SBP_STATE_IGNORE	/**< a frame is being ignored */
	} state;
} sblp_data;

void sbp(struct hardware_callbacks *hw_cb, struct protocol_callbacks *pr_cb) {
	/* store local copies of the HW structs */
	sblp_data.hw_cb = hw_cb;
	sblp_data.pr_cb = pr_cb;

	sblp_data.state = SBP_STATE_INIT;
}
