/* sbp.h - header for the spacebus protocol layer and communication
 * between layers
 */

#ifndef _SBP_H

#include "inttypes.h"

/* this struct contains the callbacks used for communication between the
 * SBP and hardware layers. Callbacks prefixed u_ are Up from HW to SBP,
 * and those marked d_ are Down from SBP to HW. same goes for variables:
 * those marked u_ are allocated/filled by HW and read by SBP, and vice
 * versa for the i_ ones.
 */
struct hw_callbacks {
	void (*u_byte_received)();
	void (*u_byte_sent)();
	void (*u_error)();

	void (*d_begin_transmission)();
	void (*d_end_transmission)();
	void (*d_send_byte)(uint8_t b);
	uint8_t (*i_receive_byte)();
} ;

/* this struct contains a pointer to a buffer to store a frame, and its
 * size.
 */

struct sbp_buffer {
	void (*u_frame_received)();
	void (*u_frame_sent)();

	void (*d_send_frame)();
	void (*d_recv_frame)();
} ;

#define _SBP_H
#endif
