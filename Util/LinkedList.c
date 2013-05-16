#include"LinkedList.h"

LLNODE*lle(void*const e)
{
	LLNODE*x = (LLNODE*)malloc(sizeof(LLNODE));
	x->e=e;
	x->n=NULL;
	return x;
}

void lladd(LLNODE*list, void*const e)
{
	if(list==NULL)return;
	LLNODE*x=list;
	while(x->n!=NULL)x=x->n;
	x->n=lle(e);
}

LLNODE*llrm(LLNODE*const list, void const*const e)
{
	if(list==NULL)return NULL;
	LLNODE*x=list;
	while(x->n!=e)
	{
		if(x->n==NULL)return NULL;
		x=x->n;
	}
	LLNODE*r=x->n;
	x->n=x->n->n;
	return r;
}
