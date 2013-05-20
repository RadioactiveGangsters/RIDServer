/* 
 * File:   util.h
 * Author: Bas
 *
 */

#ifndef UTIL_H
#define	UTIL_H

typedef struct
{
	void(*f)(void*);
} delegate;

unsigned int numlen(unsigned const int);

#endif	/* UTIL_H */

