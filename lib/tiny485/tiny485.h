#ifndef _TINY485_H

#include "sbp.h"

/* this is the only function the outside world needs to know about -
 * it fills in the callbacks needed to provide an interface
 */
void tiny485(struct hw_callbacks *cb);

#define _TINY485_C
#endif
