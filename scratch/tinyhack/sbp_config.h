/* sbp_config.h - misc config data for sbp.c/h */

#ifndef _SBP_CONFIG_H

#define SBP_BIT_LENGTH		10	/* bit length in usec */
#define SBP_HALF_BIT_TIMER	SBP_BIT_LENGTH

#define SBP_HEADER_LENGTH	 6	/* length of protocol header */
#define SBP_CHECKSUM_LENGTH	 1	/* same for checksum */

#define SBP_FLAG_ERROR	0x01
#define SBP_FLAG_ESCAPE	0x02

#define SBP_BYTE_SYNC	0b10101010	/* synchronisation byte */
#define SBP_BYTE_ESCAPE	0b01010101	/* escape byte. TODO: formalise */

#define _SBP_CONFIG_H
#endif
