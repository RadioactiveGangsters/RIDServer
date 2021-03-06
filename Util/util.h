/* 
 * File:   util.h
 * Author: Bas
 *
 */

#ifndef UTIL_H
#define	UTIL_H

#include <stdlib.h>

typedef struct
{
	void(*f)(void*);
} delegate;

unsigned int numlen(/*@+ignoresigns@*/unsigned const int);
int singlerandom(int bound);
int multirandom(int bound);

#endif	/* UTIL_H */

