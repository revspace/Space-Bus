/** \file test.c
 * \brief Generic t485 test application - sends increasing numbers to the bus on button press
 *
 * Targets an attiny MCU connected to the space bus.
 */

#include <avr/io.h>
#define F_CPU 1000000UL	// 1 MHz
#include <util/delay.h>

#include "interop.h"

#define IO_PORT PORTB
#define IO_PIN	 PINB
#define IO_DDR	 DDRB

#define LED1_PIN  PB4
#define LED2_PIN  PB3
#define SW_PIN	  PB5

struct {
	volatile int sent;
} test_data;

void byte_received(uint8_t b) {
/*	if(b & 0x01)	IO_PORT |=  _BV(LED1_PIN);
	else		IO_PORT &= ~_BV(LED1_PIN);

	if(b & 0x02)	IO_PORT |=  _BV(LED2_PIN);
	else		IO_PORT &= ~_BV(LED2_PIN);

	if(b & 0x04)	IO_PORT |=  _BV(LED4_PIN);
	else		IO_PORT &= ~_BV(LED4_PIN); */
}

void byte_sent() {
	test_data.sent=1;
}

void sync_received () {
}

void send(uint8_t b) {
	test_data.sent = 0;
	begin_transmission();
	send_byte(b);
	while(test_data.sent == 0) {
		IO_PORT |= _BV(LED2_PIN);
		IO_PORT &= ~_BV(LED2_PIN);
	}
	end_transmission();
}

int main(void) {
	uint8_t i = 0;

	hw_init();

	/* set up I/O */
	IO_DDR |=  _BV(LED1_PIN) | _BV(LED2_PIN);	/* LEDs */
	IO_DDR &= ~_BV(SW_PIN);						/* switch */

	/* enable pull-up */
	IO_PORT |= _BV(SW_PIN);
 
	while(1) {
		IO_PORT |= _BV(LED1_PIN);

		i = (i+1) % 8;
		send(i);

		IO_PORT &= ~_BV(LED1_PIN);
	}
}
