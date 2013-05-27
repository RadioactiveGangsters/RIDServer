#ifndef RkDeathRow_H
#define RkDeathRow_H

#include <stdlib.h>
#include <string.h>

typedef struct _AutoQ
{
	void*e;
	struct _AutoQ*n;
	unsigned int space;
	
} AutoQ;

AutoQ*AutoQe(void*const,unsigned int const);
AutoQ*AutoQadd(AutoQ*const,void*const);

#endif