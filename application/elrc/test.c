/** \file test.c
 * \brief Test application, sends bytes to the bus that can be interpreted
 * by the elrc application.
 *
 * Targets an attiny85 connected to the space bus.
 */

#include <avr/io.h>
#define F_CPU 1000000UL  // 1 MHz
#include <util/delay.h>

#include "interop.h"
#include "tiny485.h"

typedef struct {
  volatile int sent;
} test_data_t;

void byte_received(uint8_t b,void *data) {

}

void byte_sent(void *data) {
  test_data_t *test = (test_data_t*)data;
  test->sent=1;
}

void send(uint8_t b, struct hw_interface *cb, test_data_t *data) {
        data->sent=0;
	cb->d_begin_transmission();
	cb->d_send_byte(b);
        while(data->sent==0);
        cb->d_end_transmission();
}

int main(void) {
  struct hw_interface cb;
  test_data_t data;
  uint8_t lamp_state=0;

  /* initialize spacebus link layer */
  cb.u_byte_received=&byte_received;
  cb.u_byte_sent=&byte_sent;
  cb.c_data=(void *)&data;

  tiny485(&cb);

	// set inputs
  DDRB &= ~_BV(3);
  DDRB &= ~_BV(4);

	// enable pull-up
  PORTB |= _BV(3);
  PORTB |= _BV(4);
 
  while(1) {
	if(!(PINB&_BV(3))) {
		send(1,&cb,&data);
		_delay_ms(1000);
	}
	if(!(PINB&_BV(4))) {
		lamp_state=1-lamp_state;
		send(lamp_state?3:4,&cb,&data);
		_delay_ms(1000);
	}
	_delay_ms(100);
  }
}
