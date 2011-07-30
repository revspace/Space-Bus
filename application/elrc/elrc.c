/** \file elrc.c
 * \brief Application logic for elevator lock release coil and
 * elevator lighting.
 *
 * Targets an attiny85 with two relays connected to PB3 and PB4.
 */

#include <avr/io.h>
#include "tiny485.h"

#define ELRC_PORT	PORTB
#define ELRC_DIR	DDRB
#define ELRC_PIN	3

#define LAMP_PORT	PORTB
#define LAMP_DIR	DDRB
#define LAMP_PIN	4

// TODO: this should be userdata in the hw callback struct
static int elrc=0;
static int lamp=0;

void byte_received(uint8_t b) {
  switch(b) {
    case 1: elrc=1; break;
    case 2: elrc=0; break;
    case 3: lamp=1; break;
    case 4: lamp=0; break;
    default: break; //ignore unknown bytes
  }
}

void byte_sent(void) {
}

int main(void) {
  struct hw_callbacks cb;

  /* initialize relay i/o */
  ELRC_DIR |= (1<<ELRC_PIN);
  LAMP_DIR |= (1<<LAMP_PIN);

 
  /* initialize spacebus link layer */
  /* TODO: use SBP instead of raw frames */
  cb.u_byte_received=byte_received;
  cb.u_byte_sent=byte_sent;

  tiny485(&cb);

  while(1) {
	if(elrc) ELRC_PORT|=_BV(ELRC_PIN); else ELRC_PORT&=~_BV(ELRC_PIN);
	if(lamp) LAMP_PORT|=_BV(LAMP_PIN); else LAMP_PORT&=~_BV(LAMP_PIN);
	// waste cycles
  }
}
