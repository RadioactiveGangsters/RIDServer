/* 
 * File:   util.c
 * Author: Bas
 *
 */

#include "util.h"

unsigned int numlen(/*@+ignoresigns@*/const unsigned int t)
{
	// Beats including math.h, is also faster than log10
	return (unsigned)(t<10?1:
	                  t<100?2:
	                  t<1000?3:
	                  t<10000?4:
	                  t<100000?5:
	                  t<1000000?6:
	                  t<10000000?7:
	                  t<100000000?8:
	                  t<1000000000?9: 10);
	// no int is larger than 10
}