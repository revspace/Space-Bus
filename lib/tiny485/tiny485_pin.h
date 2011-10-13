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

#define PCINT0_vect      _VECTOR(0x0B)
#define TIM0_COMPA_vect  _VECTOR(0x0D)
#define USI_OVF_vect     _VECTOR(0x10)

#define USIBR _SFR_IO8(0x000)
#define USIBR0 0
#define USIBR1 1
#define USIBR2 2
#define USIBR3 3
#define USIBR4 4
#define USIBR5 5
#define USIBR6 6
#define USIBR7 7

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
