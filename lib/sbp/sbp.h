/** \file sbp.h
 * Header for the spacebus protocol layer and communication between
 * layers
 */

#ifndef _SBP_H

#include "inttypes.h"

/** Contains an SBP frame to be passed between layers. */
struct sbp_frame {
	uint16_t	 payload_size;
	uint8_t	*payload;
} ;

/** This struct works in the same way as the hw_callbacks struct,
 * but contains callbacks between SBLP and logical layer instead.
 */
struct protocol_callbacks {
	void (*u_frame_received)(struct sbp_frame frame);
	void (*u_frame_sent)();

	void (*d_send_frame)(struct sbp_frame frame);
	void (*d_recv_frame)();
} ;

#define _SBP_H
#endif
