/** \file sblp.h
 * Header file for the (HW-independent part of the) SpaceBus Link Protocol implementation.
 */

#ifndef _SBLP_H

#include "interop.h"

/** The only function we need to know about - this fills both interface
 * structs with the callbacks we need to operate this layer.
 */
void sblp(struct hw_interface *hw_if, struct protocol_interface *pr_if);

#define _SBLP_H
#endif
