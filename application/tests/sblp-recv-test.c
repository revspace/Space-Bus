#include <avr/io.h>
#define F_CPU 1000000UL	// 1 MHz
#include <util/delay.h>

#include "interop.h"

void frame_sent() { }

void frame_received(struct sblp_header *header, uint8_t *payload) {
	if(payload[0])	PORTB |=  _BV(PB3);
	else		PORTB &= ~_BV(PB3);

	if(payload[1])	PORTB |=  _BV(PB4);
	else		PORTB &= ~_BV(PB4);
}

int main(void) {
	hw_init();
	sblp_init();
	
	DDRB |= _BV(PB3) | _BV(PB4);

	while(1) _delay_ms(500);
}
