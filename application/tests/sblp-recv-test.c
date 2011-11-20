#include <avr/io.h>
#define F_CPU 1000000UL	// 1 MHz
#include <util/delay.h>

#include "interop.h"

void frame_sent() { }

void frame_received(struct sblp_header *header, uint8_t *payload) {
	if(payload[0])	PORTB |=  _BV(PA0);
	else		PORTB &= ~_BV(PA0);

	if(payload[1])	PORTB |=  _BV(PA1);
	else		PORTB &= ~_BV(PA1);
}

int main(void) {
	hw_init();
	sblp_init();
	
	DDRB |= _BV(PA0) | _BV(PA1);

	while(1) _delay_ms(500);
}
