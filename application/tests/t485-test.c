/** \file test.c
 * \brief Test application, sends bytes to the bus that can be interpreted
 * by the elrc application.
 *
 * Targets an attiny85 connected to the space bus.
 */

#include <avr/io.h>
#define F_CPU 1000000UL	// 1 MHz
#include <util/delay.h>

#include "interop.h"
#include "tiny485.h"

typedef struct {
	volatile int sent;
} test_data_t;

void byte_received(uint8_t b,void *data) {
	if(b & 0x01)	PORTD |=  _BV(PD3);
	else		PORTD &= ~_BV(PD3);
	
	if(b & 0x02)	PORTD |=  _BV(PD4);
	else		PORTD &= ~_BV(PD4);
	
	if(b & 0x04)	PORTD |=  _BV(PD5);
	else		PORTD &= ~_BV(PD5);
}

void byte_sent(void *data) {
	test_data_t *test = (test_data_t*)data;
	test->sent=1;
}

void send(uint8_t b, struct hw_interface *hw_if, test_data_t *data) {
	data->sent = 0;
	hw_if->d_begin_transmission();
	hw_if->d_send_byte(b);
	while(data->sent == 0);
	hw_if->d_end_transmission();
}

int main(void) {
	struct hw_interface hw_if;
	test_data_t data;
	uint8_t i = 0;

	/* initialize spacebus link layer */
	hw_if.u_byte_received=&byte_received;
	hw_if.u_byte_sent=&byte_sent;
	hw_if.c_data=(void *)&data;

	tiny485(&hw_if);

	/* set up I/O */
	DDRD |= _BV(PD3) | _BV(PD4) | _BV(PD5);	/* LEDs */
	DDRD &= ~_BV(PD2);			/* switch */

	/* enable pull-up */
	PORTD |= _BV(PD2);
 
	while(1) {
		if((PIND & _BV(PD2)) == 0) {
			i = (i+1)%8;
			send(i, &hw_if, &data);
			_delay_ms(1000);
		}
		_delay_ms(50);
	}
}
