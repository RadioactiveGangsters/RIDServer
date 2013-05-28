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

void DestroyAutoQ(AutoQ*const);

AutoQ*AutoQe(void*const,unsigned int const);
/*@null@*/AutoQ*AutoQadd(AutoQ*const,void*const);

void forautoq(AutoQ*const q,void(*cb)(AutoQ*const));

#endif