#include "sbp.h"

static struct {
	struct protocol_callbacks *cb;
} sbp_data;

void sbp(struct protocol_callbacks *cb) {
	sbp_data.cb = cb;
	
	
}
