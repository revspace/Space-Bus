#define F_CPU 1000000UL  // 1 MHz

#include <avr/io.h>
#include <util/delay.h>

#include "sb.h"

int main(void) {
	const uint8_t  msg[]		= "hello world";
	const uint16_t msg_size	= 11;

	DDRB = (1 << DO) | (1 << DEN);
	PORTB |= (1 << DEN);

	while(1)
		if(sb_idle())
			sb_send(msg_size, msg);
		else
			_udelay(100);
}
