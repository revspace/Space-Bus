/** \file tiny485.h
 * Header file for the ATTiny hardware layer.
 */

#ifndef _TINY485_H

#undef REQUIRE_SYNC	/* normally defined, only undefine for testing purposes */

/** initialise the tiny485 layer */
void tiny485_init();

#define _TINY485_C
#endif
