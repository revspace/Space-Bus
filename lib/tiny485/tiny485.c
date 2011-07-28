/* tiny485.c - interfaces to the hardware for the SBP implementation
 */

#include "sbp.h"
#include "tiny485.h"

/* the USI is 8 bits wide, the UART protocol uses 10 or so bits, so
 * we split them up and use some magic (as per application note AVR307)
 */
#define T485_STATE_BYTE2 0x01
#define T485_STATE_XMIT  0x02

/* some bit masks and stuff for said magic */
#define HIGH		0xFF
#define START_BIT	0xBF
#define STOP_BIT	0x07

static struct {
	struct hw_callbacks *cb;
	uint8_t state;
	uint8_t buf;
} t485_data;

uint8_t bit_reverse(uint8_t b) {
	uint8_t i, buf = 0x0;

	for(i=0; i<=7; i++)
		if(b & (1 << i)) buf |= (1 << (7-i));

	return b;
}

/* direct hardware interfacing convenience functions
 * do what they say on the tin - see ATTiny85 datasheet for details
 */
inline void timer_on() {
	OCR0B |= 0b00000101;	/* TODO: work out clock values etc. */
}

inline void timer_off() {
	OCR0B &= 0b11111000;
}

inline void usi_on() {
	USICR |= 0b00010100;
}

inline void usi_off() {
	USICR &= 0b11000011;
}

inline void pcint_on() {
	GIMSK |= 0b00100000;
}

inline void pcint_off() {
	GIMSK &= 0b11011111;
}

/* interface functions */

void tiny485(struct hw_callbacks *cb) {
	/* keep local copy of callback struct */
	_data.cb = cb;

	/* (u_* structs have already been filled in by upper layer) */

	cb->d_begin_transmission= &t485_begin_transmission;
	cb->d_end_transmission	= &t485_end_transmission;

	cb->d_send_byte		= &t485_send_byte;
	cb->d_receive_byte	= &t485_receive_byte;

	/* other init */
	/* TODO: sane initial values for pins used */
	/* TODO: initialise timer, usi, pcint */
}

inline void t485_begin_transmission() {
	usi_on();
	pcint_off();
}

inline void t485_end_transmission() {
	usi_off();
	pcint_on();
}

inline uint8_t t485_receive_byte() {
	return bit_reverse(t485_data.buf);
}

void t485_send_byte(uint8_t b) {
	b = bit_reverse(b);

	USIBR = HIGH & START_BIT & (b >> 2);
	buf = (b << 3) | (STOP_BIT);		/* store next byte to send it when necessary */

	t485_data.state = T485_STATE_XMIT;
	timer_on();
	usi_on();
}


/* interrupt vectors */

INT(PCINT0_vect) {
	if(USI_PORT & _BV(DIN)) {
		/* sync the timer - 3/2 to skip start bit */
		TCNT0 = (3 * T485_BIT_TIMER) / 2;
		t485_data.state = 0
		pcint_off();
		timer_on();
		usi_on();
	}
}

INT(USI_OVF_vect) {
	if(t485_data.state & T485_STATE_XMIT) {
		/* if we're transmitting, this means we need to either... */
		if(t485_data.state & T485_STATE_BYTE2) {
			timer_off();		/* stop transmitting */
			usi_off();
			t485_data.state = 0;
		} else {
			USIBR = t485_data.buf;	/* or send another byte */
			t485_data.state |= T485_STATE_BYTE2;
		}
	} else {
		usi_off();
		timer_off();
		/* load USI buffer into buf */
		t485_data.buf = USIBR;
		pcint_on();
	}
}
