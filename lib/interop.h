/** \file interop.h
 * Contains data needed for communication between layers.
 */

#ifndef _INTEROP_H

#include "inttypes.h"

/* hw layer */
extern void hw_init();				/**< initialise hardware */

extern void byte_received(uint8_t b);		/**< called when the HW layer receives a byte */
extern void byte_sent();			/**< called when the HW layer has sent a byte */
extern void sync_received();			/**< called when the HW layer sees the synchronisation sequence */

extern void begin_transmission();		/**< called when the link layer wishes to start a (potentially multi-byte) transmission. */
extern void end_transmission();			/**< called when the link layer wishes to end a (potentially multi-byte) transmission. */
extern void send_byte(uint8_t b);		/**< called when the link layer wishes to send a single byte (as part of a transmission). */
extern void send_sync();			/**< called when the link layer wishes to send a synchronisation sequence (as part of a transmission). */


/* sblp data structures */
/** a frame header */
struct sblp_header {
	uint8_t		type;
	uint16_t	length;
	uint8_t		dest;
	uint8_t		src;
} ;

#define HEADER_LENGTH 5		/**< length of the SBLP header */

/* sblp layer */
/** initialise the link-layer protocol */
extern void sblp_init();

/** the given sequence has been received as a frame */
extern void frame_received(struct sblp_header *header, uint8_t *payload);

/** the previous frame has been sent */
extern void frame_sent();

/** send the given sequence as a frame */
extern uint8_t send_frame(struct sblp_header *header, uint8_t *payload);

#define _INTEROP_H
#endif
