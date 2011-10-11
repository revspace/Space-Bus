/** \file interop.h
 * Contains data needed for communication between layers.
 */

#ifndef _INTEROP_H

#include "inttypes.h"

/* hw layer */
extern void hw_init();

extern void byte_received(uint8_t b);	/**< called when the HW layer receives a byte */
extern void byte_sent();			/**< called when the HW layer has sent a byte */
extern void sync_received();			/**< called when the HW layer sees the synchronisation sequence */

extern void begin_transmission();	/**< called when the link layer wishes to start a (potentially multi-byte) transmission. */
extern void end_transmission();	/**< called when the link layer wishes to end a (potentially multi-byte) transmission. */
extern void send_byte(uint8_t b);	/**< called when the link layer wishes to send a single byte (as part of a transmission). */
extern void send_sync();		/**< called when the link layer wishes to send a synchronisation sequence (as part of a transmission). */


/* sblp layer */
extern void sblp_init();

extern void frame_received(uint8_t *frame, uint16_t length);	/**< the given zero-terminated string has been received as a frame */
extern void frame_sent();						/**< the previous frame has been sent */

extern uint8_t send_frame(uint8_t *frame, uint16_t length);		/**< send the given zero-terminated string as a frame */

#define _INTEROP_H
#endif
