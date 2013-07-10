/* 
 * File:   util.h
 * Author: Bas
 *
 */

#ifndef UTIL_H
#define	UTIL_H

#include <math.h>

typedef struct
{
	void(*f)(void*);
} delegate;

unsigned int numlen(/*@+ignoresigns@*/unsigned const int);
int random(int range);

#endif	/* UTIL_H */

