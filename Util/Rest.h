#ifndef REST_H
#define	REST_H

#include <stdlib.h>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

void Rest(const unsigned int);

#endif	/* REST_H */

