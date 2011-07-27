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
 * TODO: see many comments, solve atomicity issues
 */

#include <inttypes.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include "sbp_config.h"
#include "sbp.h"

typedef enum {
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
} sbp_state_t;

/* internal state, do not rely on this from outside the library */
/* TODO: separate out frame info from state info */
static struct {
	sbp_state_t	state;	/* current state of interface */

	uint16_t	index;	/* index into frame (0-5 is header, rest is payload) */
	uint16_t	size;
	
	uint8_t	flags;		/* some housekeeping data. can contain any of the SBP_FLAG_* flags. */
	uint8_t	address;	/* this node's address */

	sbp_frame_t	frame;		/* a buffer for the frame */
} _data;

/* TODO: fill in these four */
/* (temporarily) disable the USI */
void disable_usi() {
	
}

/* (temporarily) enable the USI */
void enable_usi() {
	
}


/* (temporarily) disable timer 0's match interrupt */
void disable_tim0_int() {
	
}

/* (temporarily) enable timer 0's match interrupt */
void enable_tim0_int() {
	
}


/* initialise space bus lib. */
/* TODO: implement commands & control flow from comments */
void sbp_init(uint8_t address) {
	/* misc initalisation */
	_data.flags	= 0;		/* no error, no overflow */
	_data.frame.msg	= 0;		/* no message yet... */
	_data.frame.length	= 0;	/* ...so size is 0 */
	_data.index	= 0;
	_data.address	= address;

	/* set up pin change interrupt */
	
	/* set up timer 0 for USI & sync */
	
	/* set up USI */
	
	/* transition to SBP_INIT */
	_data.state	= SBP_INIT;	/* first we must sync to the bus */
	disable_usi();
	enable_tim0_int();
}

/* is transmission busy? */
uint8_t sbp_idle_get() {
	return _data.state == SBP_IDLE;
}

uint8_t sbp_error_get() {
	return data.flags & SBP_FLAG_ERROR;
}

void sbp_error_clear() {
	data.flags &= ~SBP_FLAG_ERROR;
}

/* send a message. buffer must remain valid until sbp_busy() returns false. */
void sbp_send(uint8_t dst, uint8_t type, uint16_t length, const uint8_t *msg) {
	if(_data.state == SBP_IDLE) {
		/* transition to SBP_XMIT_HEADER */
		_data.state	= SBP_XMIT_HEADER;
		_data.index	= 0;

		_data.frame.sync	= SBP_BYTE_SYNC;
		_data.frame.type	= type;
		_data.frame.length	= length + SBP_HEADER_LENGTH + SBP_CHECKSUM_LENGTH;
		_data.frame.dst		= dst;
		_data.frame.src		= _data.address;

		_data.frame.payload	= msg;

		_data.frame.checksum	= 0x0;	/* checksum is computed on the fly during transmission */

		/* disable PCINT0 */
		/* enable USI, load first byte */
	}
}

/* pin change interrupt for data in pin
 * synchronises the per-bit timer to the bus
 */
ISR(PCINT0_vect) {
	/* sync USI clock */
	TCNT0 = SBP_HALF_BIT_TIMER;
}

/* timer interrupt for per-bit timer
 * only enabled when in IDLE or INIT states - this interrupt provides bus synchronisation in software for these
 */
ISR(TIM0_COMPA_vect) {
	switch(_data.state) {
		case SBP_INIT:
			/* this state makes sure the device is synchronised to the bus first by receiving on a per-bit basis and checking for the sync byte */

			/* shift bit into byte buffer */
			_data.buf =	  (_data.buf << 1)
					| (SB_PORT & SB_DIN)?1:0;

			/* test for sync byte */
			/* if we're escaped, ignore next byte - this is not an actual sync */
			if(_data.flags & SBP_FLAG_ESCAPE) {
				/* ignore this byte and unset escape flag */
				_data.flags &= ~SBP_FLAG_ESCAPE;
			} else {
				/* otherwise we may have a genuine sync - handle this */
				
				if(_data.buf == SBP_BYTE_SYNC) {
					/* transition to SBP_RECV_HEADER */
					disable_timer_int();
					enable_usi();

					_data.index = 1;		/* we've already seen the sync byte so skip that */
					_data.state = SBP_RECV_HEADER;
				}

				/* test for escape */
				if(_data.buf == SBP_BYTE_ESCAPE) {
					_data.flags |= SBP_FLAG_ESCAPE;
				}
			}

			break;

		case SBP_IDLE:
			/* shift bit into byte buffer */
			_data.buf =	  (_data.buf << 1)
					| (SB_PORT & SB_DIN)?1:0;

			/* detect sync byte (escapedness shouldn't matter as we're synced to the bus) */
			if(_data.buf == SBP_BYTE_SYNC) {
				/* transition to SBP_RECV_HEADER */
				disable_timer_int();
				enable_usi();

				_data.index = 1;		/* we've already seen the sync byte so skip that */
				_data.state = SBP_RECV_HEADER;
			}

			break;

		default:
			return;
	}
}

/* USI overflow: one byte has been read or sent */
/* TODO: implement commands & control flow from comments */
ISR(USI_OVF_vect) {
	switch(_data.state) {
		case SBP_INIT:
		case SBP_IDLE:
			/* we skip init and idle, they use PCINT0 to sync with the bus */
			return;


		/* transmit stages */
		case SBP_XMIT_HEADER:
			/* transmit next byte of header */
			switch(_data.index) {
				/* load the correct field into the buffer */
				case 0:
					/* synchronisation byte. shouldn't occur (we transmit
					 * it when going into send mode) but handle it anyway
					 */
					USIBR = _data.frame.sync;
					break;

				case 1:
					/* frame type */
					USIBR = _data.frame.type;
					break;

				case 2:
					/* low byte of length */
					USIBR = _data.frame.len & 0xFF;
					break;

				case 3:
					/* high byte of length */
					USIBR = (_data.frame.len >> 8) & 0xFF;
					break;

				case 4:
					/* destination addr */
					USIBR = _data.frame.dst;
					break;

				case 5:
					/* source addr */
					USIBR = _data.frame.src;
					break;

				default:
					/* end of header, transition to SBP_XMIT_PAYLOAD */
					_data.state = SBP_XMIT_PAYLOAD;
					break;
			}
			break;

		case SBP_XMIT_PAYLOAD:
			/* transmit byte of payload */
			USIBR = _data.frame.payload[_data.index - SBP_HEADER_LENGTH];
			
			/* calculate checksum */
			if(_data.index == _data.size + SBP_HEADER_LENGTH) {
				_data.state = SBP_XMIT_CHECKSUM;
			}
			break;

		case SBP_XMIT_CHECKSUM:
			/* transmit checksum byte */
			USIBR = _data.checksum;

			/* transition to SBP_IDLE */
			_data.state = SBP_IDLE;
			disable_usi();
			enable_tim0_int();
			break;


		/* receive stages */
		case SBP_RECV_HEADER:
			/* read in byte of header */
			switch(_data.index) {
				/* load the buffer into the correct field (fields are as per the spec) */

				case 0:
					/* synchronisation byte. shouldn't occur (we read it
					 * in during INIT and IDLE states) but handle it anyway
					 */
					_data.frame.sync = USIBR;
					break;

				case 1:
					/* frame type */
					_data.frame.type = USIBR;
					/* TODO: can we handle this? if not, ignore */
					break;

				case 2:
					/* low byte of length */
					_data.frame.len = USIBR;
					break;

				case 3:
					/* high byte of length */
					_data.frame.len |= (USIBR << 8);
					break;

				case 4:
					/* destination addr */
					_data.frame.dst = USIBR;
					/* TODO: is this for us? if not, ignore */
					break;

				case 5:
					/* source addr & end of header, transition to SBP_RECV_PAYLOAD */
					_data.frame.src = USIBR;
					_data.state = SBP_RECV_PAYLOAD;
					break;

				default:
					/* end of header, transition to SBP_RECV_PAYLOAD */
					/* (odd condition but handle it anyway */
					_data.state = SBP_RECV_PAYLOAD;
					break;
			}

			_data.index++;
			break;

		case SBP_RECV_PAYLOAD:
			/* read in byte of payload */
			_data.index++;

			/* calculate checksum */
			/* end of payload? start reading in checksum */
			break;

		case SBP_RECV_CHECKSUM:
			/* read in checksum */
			/* matches own calculation? signal new frame */
			/* otherwise, signal error */
			
			/* transition to SBP_IDLE */
			_data.state = SBP_IDLE;
			break;

		case SBP_RECV_IGNORE:
			_data.index++;

			if(_data.index == _data.size + SBP_HEADER_LENGTH + SBP_CHECKSUM_LENGTH) {
				/* transition to SBP_IDLE */
				_data.state = SBP_IDLE;
			}
			break;

		default:
			/* unknown state, force into initialisation */
			_data.state = SBP_INIT;
			return;
	}
}
