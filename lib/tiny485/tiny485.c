/** \file tiny485.c
 * \brief An SBLP hardware-side implementation for the ATTiny85.
 *
 * This file implements the hardware-dependent parts of the SBLP protocol.
 * It is specific to the Atmel ATTiny85 microcontroller.
 *
 * \todo the decision for uart-mode transmission has not been
 * finalised. Make sure this code is correct when it has.
 */

#include <avr/interrupt.h>
#include <avr/io.h>

#include "../sbp/sbp.h"
#include "tiny485.h"

#define T485_BIT_TIMER 104

/* the USI is 8 bits wide, the UART protocol uses 10 or so bits, so
 * we split them up and use some magic (as per application note AVR307)
 */
#define T485_STATE_BYTE2 0x01	/** internal state flag: second byte of UART pair? */
#define T485_STATE_XMIT  0x02	/** internal state flag: transmitting or receiving? */

/* some bit masks and stuff for said magic */
#define HIGH		0xFF
#define START_BIT	0xBF
#define STOP_BIT	0x07

static struct {
	struct hw_callbacks *cb;
	uint8_t state;
	uint8_t buf;
} t485_data;

/** reverse bits in a byte. necessary for host/wire bit order switching.  */
uint8_t bit_reverse(uint8_t b) {
	uint8_t i, buf = 0x0;

	for(i=0; i<=7; i++)
		if(b & (1 << i)) buf |= (1 << (7-i));

	return b;
}

/* direct hardware interfacing convenience functions
 * do what they say on the tin - see ATTiny85 datasheet for details
 */

/** Turn timer 0 on.
 * timer0 is switched by connecting/disconnecting the prescaler
 * (clk_io / 8) to/from the timer0 clock.
 */
inline void timer_on() {
	TCCR0B |= 0b00000010;
}

/** Turn timer 0 off. */
inline void timer_off() {
	TCCR0B &= 0b11111000;
}

/** Turn the USI on.
 * the USI is switched by turning the entire USI and its clock on/off
 */
inline void usi_on() {
	USICR |= 0b00010100;
}

/** Turn the USI off. */
inline void usi_off() {
	USICR &= 0b11000011;
}

/** Turn the pin-change interrupt on.
 * This is done by switching the entire pin-change interrupt.
 */
inline void pcint_on() {
	GIMSK |= 0b00100000;
}

/** Turn the pin-change interrupt off. */
inline void pcint_off() {
	GIMSK &= 0b11011111;
}


/* interface functions */
/** Start a transmission.
 * This will tell the line driver to take the bus off high-impedance
 * mode so other nodes can see our transmission. It also turns the
 * pin-change interrupt off so we don't get distracted by seeing our own
 * data.
 */
void t485_begin_transmission() {
	DEN_PORT |= _BV(DEN);
	pcint_off();
}

/** End a transmission.
 * This will return the bus to high-impedance mode and turn the pin-
 * change interrupt back on.
 */
void t485_end_transmission() {
	DEN_PORT &= ~_BV(DEN);
	pcint_on();
}

/** Send a single byte.
 * This will start pushing out a byte through the USI into the line
 * driver. Make sure to call the begin_transmission function before
 * this.
 */
void t485_send_byte(uint8_t b) {
	b = bit_reverse(b);

	USIDR = HIGH & START_BIT & (b >> 2);
	USISR = 0x0B;	/* wait for 16-11 = 5 bits (half the message) */

	t485_data.buf = (b << 3) | (STOP_BIT);	/* store next byte to send it when necessary */
	t485_data.state = T485_STATE_XMIT;

	timer_on();
	usi_on();
}

void tiny485(struct hw_callbacks *cb) {
	/* keep local copy of callback struct */
	t485_data.cb = cb;

	/* (u_* structs have already been filled in by upper layer) */

	cb->d_begin_transmission= &t485_begin_transmission;
	cb->d_end_transmission	= &t485_end_transmission;

	cb->d_send_byte		= &t485_send_byte;

	/** \todo other init? */
	USI_DDR |= _BV(DO);
	USI_DDR &= ~_BV(DI);
	
	DEN_DDR |= _BV(DEN);

	/* initialise timer */
	TCCR0A = 0b00000010;		/* CTC mode */
	TCCR0B = 0b00000010;		/* prescaler = 8 */
	OCR0A  = T485_BIT_TIMER;	/* interrupt every bit length */

	/* initialise USI */
	USICR = 0b01010000;

	/* initialise pin-change interrupt */
	PCMSK = 0b00000001;		/* enable for PCINT0 (DI) pin */
	GIMSK = 0b00100000;		/* enable in general */
	sei();
}


/* interrupt vectors */
/** Pin change ISR. Synchronise the receive timer to the node transmitting. */
ISR(PCINT0_vect) {
	if((USI_PORT & _BV(DI)) == 0) {
		/* start bit detected! sync the timer - sample 1/2 bit length later */
		TCNT0 = T485_BIT_TIMER / 2;
		t485_data.state = 0;
		USISR = 0x07;	/* wait for 16-7 = 9 bits (start bit + a byte) */

		pcint_off();
		timer_on();
		usi_on();
	}
}

/** USI overflow vector - process the transmitted or received byte
 * This ISR gets run whenever the USI overflows - for us, this means a
 * byte has been transmitted or received. As we're abusing the USI as a
 * hacky UART, we may need to send another byte. If not, notify the
 * layer above via the appropriate struct hw_callbacks members.
 */
ISR(USI_OVF_vect) {
	if(t485_data.state & T485_STATE_XMIT) {
		/* if we're transmitting, this means we need to either... */
		if(t485_data.state & T485_STATE_BYTE2) {
			timer_off();		/* ...stop transmitting... */
			usi_off();
			t485_data.state = 0;

			/* ...and notify the layer above... */
			sei();
			(*(t485_data.cb->u_byte_sent))(t485_data.cb->c_data);
		} else {
			USIDR = t485_data.buf;	/* ...or send another byte */
			USISR = 0x0B;		/* wait for 16-11 = 5 bits (half the message) */
			t485_data.state |= T485_STATE_BYTE2;
		}
	} else {
		/* if we're receiving, we need to handle the received byte */
		usi_off();
		timer_off();

		/* load USI buffer into buf */
		t485_data.buf = USIBR;
		pcint_on();

		/* notify higher level */
		sei();
		(*(t485_data.cb->u_byte_received))(bit_reverse(USIBR), t485_data.cb->c_data);
	}
}
