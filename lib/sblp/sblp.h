/** \file sblp.h
 * Header file for the (HW-independent part of the) SpaceBus Link Protocol implementation.
 */

#ifndef _SBLP_H

/** The only function we need to know about - this fills both interface
 * structs with the callbacks we need to operate this layer.
 */
void sblp_init();

#define _SBLP_H
#endif
