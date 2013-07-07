#ifndef RkDeathRow_H
#define RkDeathRow_H

#include <stdlib.h>
#include <string.h>

typedef struct autoQ
{
	void*e;
	struct autoQ*n;
	unsigned int space;
	
} AutoQ;

void DestroyAutoQ(AutoQ*const);

AutoQ*AutoQe(void*const,unsigned int const);
/*@null@*/AutoQ*AutoQadd(AutoQ*const,void*const);

void forautoq(AutoQ*const,void(*)(AutoQ*const));

int AutoQcount(AutoQ const*const);

#endif
