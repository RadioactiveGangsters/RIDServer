#ifndef RLINKEDLIST_H
#define RLINKEDLIST_H

#include <stdlib.h>

typedef struct _LLNODE 
{
	void*e;
	struct _LLNODE*n;
} LLNODE;

LLNODE*lle(void*const);

void lladd(LLNODE*const, void*const);

LLNODE*llrm(LLNODE*const, void const*const);

#endif
