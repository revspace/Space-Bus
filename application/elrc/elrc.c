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

typedef struct {
  int elrc;
  int lamp;
} elrc_data_t;

// TODO: this should be userdata in the hw callback struct

void byte_received(uint8_t b,void *data) {
  elrc_data_t *elrc = (elrc_data_t *)data;
  switch(b) {
    case 1: elrc->elrc=1; break;
    case 2: elrc->elrc=0; break;
    case 3: elrc->lamp=1; break;
    case 4: elrc->lamp=0; break;
    default: break; //ignore unknown bytes
  }
}

void byte_sent(void *data) {
}

int main(void) {
  struct hw_callbacks cb;
  elrc_data_t data={0,0};

  /* initialize relay i/o */
  ELRC_DIR |= (1<<ELRC_PIN);
  LAMP_DIR |= (1<<LAMP_PIN);

 
  /* initialize spacebus link layer */
  /* TODO: use SBP instead of raw frames */
  cb.u_byte_received=byte_received;
  cb.u_byte_sent=byte_sent;
  cb.c_data=&data;

  tiny485(&cb);

  while(1) {
	if(data.elrc) ELRC_PORT|=_BV(ELRC_PIN); else ELRC_PORT&=~_BV(ELRC_PIN);
	if(data.lamp) LAMP_PORT|=_BV(LAMP_PIN); else LAMP_PORT&=~_BV(LAMP_PIN);
	// waste cycles
  }
}
