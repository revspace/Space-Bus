#include <avr/io.h>
#define F_CPU 1000000UL	// 1 MHz
#include <util/delay.h>

#include "interop.h"

#define TEST_MSG		((uint8_t) "hello, world")
#define TEST_MSG_LEN	12

volatile uint8_t sent = 0;

void frame_sent() {
	sent = 1;
}

void frame_received(struct sblp_header *header, uint8_t *payload) {

}

int main(void) {
	struct sblp_header head;
	hw_init();
	sblp_init();
	
	head.type = 1;
	head.length = TEST_MSG_LEN + HEADER_LENGTH;

	while(1) {
		sent = 0;
		if(send_frame(&head, TEST_MSG))
			while(!sent) _delay_ms(50);

		_delay_ms(500);
	}
}
