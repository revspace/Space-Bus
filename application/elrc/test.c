/** \file test.c
 * \brief Test application, sends bytes to the bus that can be interpreted
 * by the elrc application.
 *
 * Targets an attiny85 connected to the space bus.
 */

#include <avr/io.h>
#define F_CPU 8000000UL  // 8 MHz
#include <util/delay.h>
#include "tiny485.h"

typedef struct {
  int sent;
} test_data_t;

void byte_received(uint8_t b,void *data) {

}

void byte_sent(void *data) {
  test_data_t *test = (test_data_t*)data;
  test->sent=1;
}

int main(void) {
  struct hw_callbacks cb;
  uint8_t byte;
  test_data_t data;

  /* initialize spacebus link layer */
  cb.u_byte_received=&byte_received;
  cb.u_byte_sent=&byte_sent;

  tiny485(&cb);

  byte=1;
  while(1) {
        data.sent=0;
	cb.d_begin_transmission();
	cb.d_send_byte(byte);
        while(data.sent==0);
        cb.d_end_transmission();
	if( ++byte > 4) byte=1;
	_delay_ms(100);
  }
}
