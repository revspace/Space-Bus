/** \file interop.h
 * Contains data structures needed for communication between layers.
 */

#ifndef _INTEROP_H

#include "inttypes.h"

/** SBLP/HW communication struct
 *
 * This struct contains the callbacks used for communication between the
 * link (SBP) and hardware layers. Callbacks prefixed u_ are up from HW to
 * SBLP, and those marked d_ are down from SBLP to HW. same goes for
 * variables: those marked u_ are allocated/filled by HW and read by
 * SBLP, and vice versa for the i_ ones.
 */
struct hw_interface {
	void  *c_data;					/**< custom pointer for link-specific data */
        void (*u_byte_received)(uint8_t b,void *d);	/**< called when the HW layer receives a byte */
        void (*u_byte_sent)(void *d);			/**< called when the HW layer has sent a byte */
	void (*u_sync_received)(void *d);		/**< called when the HW layer sees the synchronisation sequence */

        void (*d_begin_transmission)();		/**< called when the link layer wishes to start a (potentially multi-byte) transmission. */
        void (*d_end_transmission)();		/**< called when the link layer wishes to end a (potentially multi-byte) transmission. */
        void (*d_send_byte)(uint8_t b);	/**< called when the link layer wishes to send a single byte (as part of a transmission). */
} ;

/** Contains a frame to be passed between layers. */
struct frame {
	uint16_t	 payload_size;	/**< size of payload, in bytes */
	uint8_t	*payload;	/**< pointed to first byte of payload */
} ;

/** This struct works in the same way as the hw_callbacks struct,
 * but contains callbacks between link and logical layer instead.
 */
struct protocol_interface {
	void (*u_frame_received)(struct frame f);
	void (*u_frame_sent)();

	void (*d_send_frame)(struct frame f);
	void (*d_recv_frame)();
} ;

#define _INTEROP_H
#endif
