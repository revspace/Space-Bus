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

/** SBLP/HW communication struct
 *
 * This struct contains the callbacks used for communication between the
 * SBP and hardware layers. Callbacks prefixed u_ are up from HW to
 * SBLP, and those marked d_ are down from SBLP to HW. same goes for
 * variables: those marked u_ are allocated/filled by HW and read by
 * SBLP, and vice versa for the i_ ones.
 */
struct hw_callbacks {
	void (*u_byte_received)(uint8_t b);	/**< called when the HW layer receives a byte */
	void (*u_byte_sent)();			/**< called when the HW layer has sent a byte */

	void (*d_begin_transmission)();		/**< called when the SBLP layer wishes to start a (potentially multi-byte) transmission. */
	void (*d_end_transmission)();		/**< called when the SBLP layer wishes to end a (potentially multi-byte) transmission. */
	void (*d_send_byte)(uint8_t b);	/**< called when the SBLP layer wishes to send a single byte (as part of a transmission). */
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
