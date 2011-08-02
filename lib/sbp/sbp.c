#include "interop.h"
#include "sbp.h"

static struct {
	struct protocol_callbacks	*pr_cb;
	struct hardware_callbacks	*hw_cb;

	enum {
		SBP_STATE_INIT,	/**< the device is initialising and waiting to receive its first valid packet */
		SBP_STATE_IDLE,	/**< the device is idling - data can be sent at this stage */
		SBP_STATE_XMIT,	/**< a frame is being transmitted */

	} state;
} sblp_data;

void sbp(struct protocol_callbacks *cb) {
	sblp_data.pr_cb = cb;


}
