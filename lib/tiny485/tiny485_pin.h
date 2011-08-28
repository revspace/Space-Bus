/** \file tiny485.h
 * Header file for the avr hardware layer.
 */

#ifndef _TINY485_PIN_H

// pin configuration depending on processor architecture

#ifdef __AVR_ATtiny85__
#define USI_PORT	PORTB	/**< the port the USI in/output pins are on */
#define DEN_PORT	PORTB	/**< the port the data enable pin is on */
#define USI_DDR		DDRB
#define DEN_DDR		DDRB
#define DI		PB0	/**< the DI pin on the USI_PORT */
#define DO		PB1
#define DEN		PB2	/**< the data enable pin on the DEN_PORT */
#define AVR_SUPPORTED
#endif

#ifndef AVR_SUPPORTED
#ifdef 	__AVR_ATtiny4313__
#define USI_PORT	PORTB	/**< the port the USI in/output pins are on */
#define DEN_PORT	PORTB	/**< the port the data enable pin is on */
#define USI_DDR		DDRB
#define DEN_DDR		DDRB
#define DI		PB5	/**< the DI pin on the USI_PORT */
#define DO		PB6
#define DEN		PB7	/**< the data enable pin on the DEN_PORT */
#define AVR_SUPPORTED
#endif
#endif

#ifndef AVR_SUPPORTED
#ifdef 	__AVR_ATtiny2313__
#define USI_PORT	PORTB	/**< the port the USI in/output pins are on */
#define DEN_PORT	PORTB	/**< the port the data enable pin is on */
#define USI_DDR		DDRB
#define DEN_DDR		DDRB
#define DI		PB5	/**< the DI pin on the USI_PORT */
#define DO		PB6
#define DEN		PB7	/**< the data enable pin on the DEN_PORT */
#define AVR_SUPPORTED
#endif
#endif

#ifndef AVR_SUPPORTED
#error "unsupported avr architecture"
#endif

#define _TINY485_PIN_C
#endif
