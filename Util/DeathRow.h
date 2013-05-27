#ifndef RDeathRow_H
#define RDeathRow_H

#include <stdlib.h>
#include <string.h>

typedef struct _AutoQ
{
	void*e;
	struct _AutoQ*n;
	unsigned int space;

} AutoQ;

AutoQ*LABLAQe(void*const,AutoQ*const,int const);
AutoQ*LABLAQadd(AutoQ*const,void*const);

#endif