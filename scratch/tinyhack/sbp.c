/* sbp.c - space bus protocol library implementation
 * 
 * This library implementation consists of a great big old state machine.
 * It starts out in SB_INIT, where it waits for an (unescaped) sync byte
 * to make sure it is synchronised with the bus properly. Then it receives
 * the message that comes after it by progressing through the SB_RECV
 * states, and drops into SB_IDLE. From there new bus data will put it
 * into SB_RECV_* again and new client data will put it into SB_XMIT_*
 * states.
 * 
 * todo: see many comments, solve atomicity issues
 */

#include <inttypes.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include "sbp.h"

#define MAX_FRAME_SIZE	255	/* maximum frame size in bytes (unescaped) */
#define BIT_LENGTH	 10	/* bit length in usec */

#define SBP_FLAG_ERROR	0x01
#define SBP_FLAG_ESCAPE	0x02

#define SBP_BYTE_SYNC	0b10101010
#define SBP_BYTE_ESCAPE	0b01010101	// todo: formalise

/* internal state, do not rely on this from outside the library */
// TODO: separate out frame info from state info
static struct sbp_state {
	enum {
		/* device init*/
		SBP_INIT,

		/* bus idle */
		SBP_IDLE,

		/* transmission stages */
		SBP_XMIT_HEADER,
		SBP_XMIT_PAYLOAD,
		SBP_XMIT_CHECKSUM,

		/* receive stages */
		SBP_RECV_HEADER,
		SBP_RECV_PAYLOAD,
		SBP_RECV_CHECKSUM,

		SBP_RECV_IGNORE		/* ignore rest of frame */
	} state;

	uint8_t flags;
	uint8_t addr;

	uint8_t *msg;			/* pointer to msg buffer */
	uint16_t index;		/* index into current chunk (header, payload etc.) in bytes */
	uint16_t size;			/* size of payload (header is fixed) in bytes */

	uint8_t header[6];		/* the frame header */
} _state;

/* initialise space bus lib. */
// TODO: implement commands & control flow from comments
void sbp_init(uint8_t address) {
	_state.state	= SBP_INIT;
	_state.flags	= 0;
	_state.msg	= 0;
	_state.size	= 0;
	_state.index	= 0;
	_state.flags	= 0;

	// fill header

	// set up pin change interrupt
	// set up USI timer
}

/* is transmission busy? */
uint8_t sbp_idle() {
	return _state.state == SBP_IDLE;
}

/* send a message. buffer must remain valid until sbp_busy() returns false. */
void sbp_send(uint16_t size, const uint8_t *msg) {
	if(_state.state == SBP_IDLE) {
		_state.state	= SBP_XMIT_HEADER;
		_state.msg	= msg;
		_state.size	= size;
		_state.index	= 0;
		
		// prepare header
		// disable pin change interrupt
		// enable timers
	}
}

/* pin change interrupt for data in pin */
// TODO: implement commands & control flow from comments
ISR(PCINT0_vect) {
	switch(_state.state) {
		case SBP_INIT:
			// detect unescaped sync, go to recv header
			break;

		case SBP_IDLE:
			// detect unescaped sync, go to recv header
			break;

		default:
			return;
	}

	// sync USI clock
	// sync from init/idle to bus?
}

/* USI overflow: one byte has been read */
// TODO: implement commands & control flow from comments
ISR(USI_OVF_vect) {
	switch(_state.state) {
		/* we skip init and idle, they use PCINT0 to sync with the bus */

		case SBP_XMIT_HEADER:
			// transmit byte of header
			// end of header? if so, start transimitting payload
			break;

		case SBP_XMIT_PAYLOAD:
			// transmit byte of payload
			// calculate checksum
			// end of payload? if so, start transmitting checksum
			break;

		case SBP_XMIT_CHECKSUM:
			// transmit checksum byte
			// go to idle
			break;

		/* receive stages */
		case SBP_RECV_HEADER:
			// read in byte of header
			// end of header? if address match, start reading in payload
			// otherwise ignore rest of message
			break;

		case SBP_RECV_PAYLOAD:
			// read in byte of payload
			// calculate checksum
			// end of payload? start reading in checksum
			break;

		case SBP_RECV_CHECKSUM:
			// read in checksum
			// matches own calculation? signal 'upstairs'
			// otherwise, signal error
			// go to bus idle
			break;

		case SBP_RECV_IGNORE:
			// count down to end of message + payload byte
			// done? go to idle
			break;

		default:
			return;
	}
}
