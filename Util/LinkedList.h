#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include<stdlib.h>

typedef struct _LLNODE 
{
	void*e;
	struct _LLNODE*n;
}LLNODE;

LLNODE lle(void const*const e);

void lladd(LLNODE*const list, void const*const e);

LLNODE*llrm(LLNODE*const list, void const*const e);

#endif
