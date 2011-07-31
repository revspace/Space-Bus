/** \file test.c
 * \brief Test application, sends bytes to the bus that can be interpreted
 * by the elrc application.
 *
 * Targets an attiny85 connected to the space bus.
 */

#include <avr/io.h>
#define F_CPU 8000000UL  // 1 MHz
#include <util/delay.h>
#include "tiny485.h"

// TODO: this should be userdata in the hw callback struct
static int sent=0;

void byte_received(uint8_t b) {
}

void byte_sent(void) {
  sent=1;
}

int main(void) {
  struct hw_callbacks cb;
  uint8_t byte;

  /* initialize spacebus link layer */
  cb.u_byte_received=byte_received;
  cb.u_byte_sent=byte_sent;

  tiny485(&cb);

  byte=1;
  while(1) {
        sent=0;
	cb.d_begin_transmission();
	cb.d_send_byte(byte);
        while(sent==0);
        cb.d_end_transmission();
	if( ++byte > 4) byte=1;
	_delay_ms(5000);
  }
}
