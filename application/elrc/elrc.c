/** \file elrc.c
 * \brief Application logic for elevator lock release coil and
 * elevator lighting.
 *
 * Targets an attiny85 with two relays connected to PB3 and PB4.
 */

#include <avr/io.h>
#define F_CPU 1000000UL  // 1 MHz
#include <util/delay.h>
#include "tiny485.h"

#define ELRC_PORT	PORTB	/**< Port to which the ELRC is connected */
#define ELRC_DIR	DDRB
#define ELRC_PIN	4

#define LAMP_PORT	PORTB
#define LAMP_DIR	DDRB
#define LAMP_PIN	3

#define COIL_DELAY	10000

/** \brief context data to be passed to callbacks */
typedef struct {
  volatile int elrc; /**< requested coil relay status (reset to 0 after timeout) */
  volatile int lamp; /**< requested lamp relay status */
} elrc_data_t;

/*
** \brief Callback function for received bytes.
**
** Function called when a byte is received, runs in interrupt
** context.
** Interprets the byte, and takes action accordingly.
**
** @param b the byte that was received
** @param data user data passed in the initialization function
**
*/
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

/*
** \brief Callback function for received bytes.
**
** Function called when a byte is sent, runs in interrupt
** context.
** Does nothing right now.
**
** @param data user data passed in the initialization function
**
*/
void byte_sent(void *data) {
  elrc_data_t *elrc = (elrc_data_t *)data;
}

int main(void) {
  struct hw_callbacks cb;
  elrc_data_t data={0,0};
  int counter=0;

  /* initialize relay i/o */
  ELRC_DIR |= (1<<ELRC_PIN);
  LAMP_DIR |= (1<<LAMP_PIN);

  /* reset relays */
  ELRC_PORT &= ~_BV(ELRC_PIN);
  LAMP_PORT &= ~_BV(LAMP_PIN);

  /** initialize spacebus link layer
   ** \todo use SBP instead of raw frames */
  cb.u_byte_received=byte_received;
  cb.u_byte_sent=byte_sent;
  cb.c_data=&data;

  tiny485(&cb);
  
  while(1) {
	if(data.elrc) {
		counter=COIL_DELAY/100;
		data.elrc=0;
	}
	if(counter) {
		ELRC_PORT|=_BV(ELRC_PIN);
		counter--;
	} else {
		ELRC_PORT&=~_BV(ELRC_PIN);
	}
	if(data.lamp) LAMP_PORT|=_BV(LAMP_PIN); else LAMP_PORT&=~_BV(LAMP_PIN);
	// waste cycles
	_delay_ms(100);
  }
}
