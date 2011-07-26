/* sbp_test.c - test device for the space bus protocol library
 */

#include <inttypes.h>

#include <util/delay.h>

#include "sbp.h"

#define SBP_ADDR 0x10

int main(void) {
	const uint8_t  msg[]		= "hello world";	/* message to send */
	const uint16_t msg_size	= 11;			/* length of same */

	/* init sbp */
	sbp_init(SBP_ADDR);

	while(1)		/* keep sending the same message */
		if(sbp_idle())
			sbp_send(msg_size, msg);
		else
			_delay_us(100);
}
