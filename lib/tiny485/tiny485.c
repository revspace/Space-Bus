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
	uint8_t buf[2];
} t485_data;

uint8_t hton(uint8_t b) {
	uint8_t i, buf = 0x0;

	for(i=0; i<=7; i++)
		if(b & (1 << i)) buf |= (1 << (7-i));
	
	return b;
}

void tiny485(struct hw_callbacks *cb) {
	/* keep local copy of callback struct */
	_data.cb = cb;

	/* (u_* structs have already been filled in by upper layer) */

	cb->d_begin_transmission= &tiny485_begin_transmission;
	cb->d_end_transmission	= &tiny485_end_transmission;

	cb->d_send_byte		= &tiny485_send_byte;
	cb->d_receive_byte	= &tiny485_receive_byte;
}

void t485_begin_transmission() {
	/* turn on DE, turn off pcint */
}

void t485_end_transmission() {
	/* turn off DE, turn on pcint */
}


uint8_t t485_receive_byte() {
	return buf[0];
}

void t485_send_byte(uint8_t b) {
	b = hton(b);

	USIBR = HIGH & START_BIT & (b >> 2);
	buf = (b << 3) | (STOP_BIT);		/* store next byte to send it when necessary */

	t485_data.state = T485_STATE_XMIT;

	timer_on();
	pcint_off();
	usi_on();
}

INT(PCINT0_vect) {
	usi_on();
	pcint_off();
}

INT(TIM0_COMPA_VECT) {
	if(t485_data.state & T84_STATE_XMIT)
		if(t485_data.state & T84_STATE_BYTE2) {
			timer_off();
			usi_off();
			pcint_on();
			t485_data.state = 0;
		} else {
			USIBR = buf;
		}
}
