/** \file tiny485.h
 * Header file for the avr hardware layer.
 */

#ifndef _TINY485_PIN_H

/**
 * pin configuration depending on processor architecture
 */

/*************************
 * 	ATTINY85
 *************************/
#ifdef __AVR_ATtiny85__
#define USI_PORT	PORTB	/**< the port the USI in/output pins are on */
#define DEN_PORT	PORTB	/**< the port the data enable pin is on */
#define USI_DDR		DDRB
#define DEN_DDR		DDRB
#define DI		PB0	/**< the DI pin on the USI_PORT */
#define DO		PB1
#define DEN		PB2	/**< the data enable pin on the DEN_PORT */

/* direct hardware interfacing convenience functions
 * do what they say on the tin - see relevant AVR datasheets for details
 */

/* process a character into the first or second xmit bytes -- see AVR307 */
#define FIRST_XMIT_BYTE(b)       ((b) >> 1)
#define SECOND_XMIT_BYTE(b)     (((b) << 4) | 0x0F)

/* timer0 is switched by connecting/disconnecting the prescaler
 * (clk_io / 8) to/from the timer0 clock.
 */
#define TIM0_ON()       TCCR0B |= 0x02  /* 0b00000010 */        /**< Turn timer 0 on. */
#define TIM0_OFF()      TCCR0B &= 0xF8  /* 0b11111000 */        /**< Turn timer 0 off. */

/* the USI is switched by turning the entire USI and its clock on/off */
#define USI_ON()        USICR |= 0x14   /* 0b00010100 */        /**< Turn the USI on. */
#define USI_OFF()       USICR &= 0xC3   /* 0b11000011 */        /**< Turn the USI off. */


/* PCINT0 is switched by switching the entire pin-change interrupt. */
#define PCINT0_ON()     GIMSK |= 0x20   /* 0b00100000 */        /**< Turn pin-change interrupt 0 on */
#define PCINT0_OFF()    GIMSK &= 0xDF   /* 0b11011111 */        /**< Turn pin-change interrupt 0 off */

/* enable pin change interrupt for the DI pin */
#define PCINTDI_ON()	PCMSK |= 0x01	/* 0b00000001 */	/**< Turn PCINT0 interrupt 0 on */
#define PCINTDI_OFF()	PCMSK &= 0xFE	/* 0b11111110 */	/**< Turn PCINT0 interrupt 0 off */

/* The timer interrupt is switched by setting its bit in the timer interrupt mask. */
#define TIM0INT_ON()    TIMSK |= 0x02   /* 0b00000010 */        /**< Turn the timer interrupt on. */
#define TIM0INT_OFF()   TIMSK &= 0xFD   /* 0b11111101 */        /**< Turn the timer interrupt off. */

/* load a counter value into the USI counter. */
#define USICOUNTER(n)   USISR = ((USISR & 0xF0) | (n))

#define AVR_SUPPORTED
#endif // __AVR_ATtiny85__

/*************************
 * 	ATTINY4313
 *       (broken)
 *************************/
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

/*************************
 * 	ATTINY2313
 *      (untested)
 *************************/
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

/*************************
 * 	ATTINY44
 *************************/
#ifndef AVR_SUPPORTED
#ifdef __AVR_ATtiny44__
#define USI_PORT	PORTA	/**< the port the USI in/output pins are on */
#define DEN_PORT	PORTA	/**< the port the data enable pin is on */
#define USI_DDR		DDRA
#define DEN_DDR		DDRA
#define DI		PA6	/**< the DI pin on the USI_PORT */
#define DO		PA5
#define DEN		PA4	/**< the data enable pin on the DEN_PORT */

/* direct hardware interfacing convenience functions
 * do what they say on the tin - see relevant AVR datasheets for details
 */

/* process a character into the first or second xmit bytes -- see AVR307 */
#define FIRST_XMIT_BYTE(b)       ((b) >> 1)
#define SECOND_XMIT_BYTE(b)     (((b) << 4) | 0x0F)

/* timer0 is switched by connecting/disconnecting the prescaler
 * (clk_io / 8) to/from the timer0 clock.
 */
#define TIM0_ON()       TCCR0B |= 0x02  /* 0b00000010 */        /**< Turn timer 0 on. */
#define TIM0_OFF()      TCCR0B &= 0xF8  /* 0b11111000 */        /**< Turn timer 0 off. */

/* the USI is switched by turning the entire USI and its clock on/off */
#define USI_ON()        USICR |= 0x14   /* 0b00010100 */        /**< Turn the USI on. */
#define USI_OFF()       USICR &= 0xC3   /* 0b11000011 */        /**< Turn the USI off. */


/* PCINT0 is switched by switching the entire pin-change interrupt. */
#define PCINT0_ON()     GIMSK |= 0x10   /* 0b00010000 */        /**< Turn pin-change interrupt 0 on */
#define PCINT0_OFF()    GIMSK &= 0xEF   /* 0b11101111 */        /**< Turn pin-change interrupt 0 off */

/* enable pin change interrupt for the DI pin */
#define PCINTDI_ON()	PCMSK0 |= 0x80	/* 0b10000000 */	/**< Turn PCINT6 on */
#define PCINTDI_OFF()	PCMSK0 &= 0x7F	/* 0b01111111 */	/**< Turn PCINT6 off */

/* The timer interrupt is switched by setting its bit in the timer interrupt mask. */
#define TIM0INT_ON()    TIMSK |= 0x01   /* 0b00000001 */        /**< Turn the timer interrupt on. */
#define TIM0INT_OFF()   TIMSK &= 0xFE   /* 0b11111110 */        /**< Turn the timer interrupt off. */

/* load a counter value into the USI counter. */
#define USICOUNTER(n)   USISR = ((USISR & 0xF0) | (n))


#define AVR_SUPPORTED
#endif
#endif // __AVR_ATtiny44__

#ifndef AVR_SUPPORTED
#error "unsupported avr architecture"
#endif

#define _TINY485_PIN_C
#endif
