/** \file interop.h
 * Contains data needed for communication between layers.
 */

#ifndef _INTEROP_H

#include "inttypes.h"

/** \todo redocument */
extern void byte_received(uint8_t b);	/**< called when the HW layer receives a byte */
extern void byte_sent();			/**< called when the HW layer has sent a byte */
extern void sync_received();			/**< called when the HW layer sees the synchronisation sequence */

extern void begin_transmission();		/**< called when the link layer wishes to start a (potentially multi-byte) transmission. */
extern void end_transmission();		/**< called when the link layer wishes to end a (potentially multi-byte) transmission. */
extern void send_byte(uint8_t b);		/**< called when the link layer wishes to send a single byte (as part of a transmission). */

/** \todo redocument */
struct frame {
	uint16_t length;
	uint8_t *payload;
} ;

extern void frame_received(struct frame f);
extern void frame_sent();

extern void send_frame(struct frame f);
extern void recv_frame();

#define _INTEROP_H
#endif
