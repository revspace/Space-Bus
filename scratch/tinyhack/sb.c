#include <avr/interrupt.h>
#include <avr/io.h>

#include "sb.h"

#define MAX_FRAME_SIZE	255	/* maximum frame size in bytes (unescaped) */
#define BIT_LENGTH	 10	/* bit length in usec */

#define SB_FLAG_ERROR	0x01
#define SB_FLAG_ESCAPE	0x02

#define SB_BYTE_SYNC	0b10101010
#define SB_BYTE_ESCAPE	0b01010101	// todo: formalise

/* internal state, do not rely on this from outside the library */
static struct sb_state {
	enum {
		/* device init*/
		SB_INIT,

		/* bus idle */
		SB_IDLE,

		/* transmission stages */
		SB_XMIT_HEADER,
		SB_XMIT_PAYLOAD,
		SB_XMIT_CHECKSUM,

		/* receive stages */
		SB_RECV_HEADER,
		SB_RECV_PAYLOAD,
		SB_RECV_CHECKSUM,

		SB_RECV_IGNORE		/* ignore rest of frame */
	} state;

	uint8_t flags;

	uint8_t *msg;			/* pointer to msg buffer */
	uint16_t index;		/* index into current chunk (header, payload etc.) in bytes */
	uint16_t size;			/* size of payload (header is fixed) in bytes */

	uint8_t header[6];		/* the frame header */
} _state;

/* initialise space bus lib. */
void sb_init() {
	_state.state	= SB_INIT;
	_state.flags	= 0;
	_state.msg	= NULL;
	_state.size	= 0;
	_state.index	= 0;
	_state.flags	= 0;
	_state.header	= { 0, 0, 0, 0, 0, 0 };

	// set up pin change interrupt
	// set up USI timer
}

/* is transmission busy? */
uint8_t sb_idle() {
	return _state.state == SB_IDLE;
}

/* send a message. buffer must remain valid until sb_busy() returns false. */
void sb_send(uint16_t size, const uint8_t *msg) {
	if(_state.state == SB_IDLE)
		_state.state	= SB_XMIT_HEADER;
		_state.msg	= msg;
		_state.size	= size;
		_state.index	= 0;

		// load sync byte
		// disable pin change interrupt
		// enable timers
	}
}

/* pin change interrupt for data in pin */
ISR(PCINT0_vect) {
	switch(_state.state) {
		case SB_INIT:
			// detect unescaped sync, go to recv header
			break;

		case SB_IDLE:
			// if sync go to recv header
			break;

		default:
			return;
	}

	// sync USI clock
	// sync from init/idle to bus?
}

/* USI overflow: one byte has been read */
ISR(USI_OVF_vect) {
	switch(_state.state) {
		/* we skip init and idle, they use PCINT0 to sync with the bus */

		case SB_XMIT_HEADER:
			// transmit byte of header
			// end of header? if so, start transimitting payload
			break;

		case SB_XMIT_PAYLOAD:
			// transmit byte of payload
			// calculate checksum
			// end of payload? if so, start transmitting checksum
			break;

		case SB_XMIT_CHECKSUM:
			// transmit checksum byte
			// go to idle
			break;

		/* receive stages */
		case SB_RECV_HEADER:
			// read in byte of header
			// end of header? if address match, start reading in payload
			// otherwise ignore rest of message
			break;

		case SB_RECV_PAYLOAD:
			// read in byte of payload
			// calculate checksum
			// end of payload? start reading in checksum
			break;

		case SB_RECV_CHECKSUM:
			// read in checksum
			// matches own calculation? signal 'upstairs'
			// otherwise, signal error
			// go to bus idle
			break;

		case SB_RECV_IGNORE:
			// count down to end of message + payload byte
			// done? go to idle
			break;

		default:
			return;
}
