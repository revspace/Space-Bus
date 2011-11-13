#include <avr/io.h>
#define F_CPU 1000000UL	// 1 MHz
#include <util/delay.h>

#include "interop.h"


uint8_t test_data[2] = {0, 0};
#define TEST_DATA_LEN 2
volatile uint8_t sent = 0;

void frame_sent() {
	sent = 1;
}

void frame_received(struct sblp_header *header, uint8_t *payload) {
	/* empty for now */
}

int main(void) {
	struct sblp_header head;
	hw_init();
	sblp_init();
	
	head.type = 1;
	head.length = TEST_DATA_LEN + HEADER_LENGTH;

	while(1) {
		if(test_data[0]) {
			test_data[0] = 0;
			test_data[1] = 1;
		} else {
			test_data[0] = 1;
			test_data[1] = 0;
		}
		sent = 0;
		if(send_frame(&head, test_data))
			while(!sent) _delay_ms(50);

		_delay_ms(1000);
	}
}
