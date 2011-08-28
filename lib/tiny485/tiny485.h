/** \file tiny485.h
 * Header file for the ATTiny85 hardware layer.
 */

#ifndef _TINY485_H

#define REQUIRE_SYNC	// normally defined, only undefine for testing purposes

#include "../sblp/interop.h"

/** Initialise hardware layer and fill in callbacks.
 *
 * This is the only function the outside world needs to know about -
 * it fills in the callbacks needed to provide an interface to the SBLP
 * (or other, for that matter) layer, then initialises itself. After
 * this, the callbacks specified in the struct hw_callbacks are valid
 * and can be called for further interaction with this layer.
 */
void tiny485(struct hw_interface *cb);

#define _TINY485_C
#endif
