#ifndef RLINKEDLIST_H
#define RLINKEDLIST_H

#include <stdlib.h>
#include <string.h>

typedef struct _LLNODE 
{
	void*e;
	struct _LLNODE*n;
} LLNODE;

/*@null@*/LLNODE*lle(void*const);

void lladd(LLNODE*const, void*const);

/*@null@*/LLNODE*llrm(LLNODE*const, void const*const);

#endif
