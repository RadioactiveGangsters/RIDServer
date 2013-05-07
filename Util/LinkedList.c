#include"LinkedList.h"

LLNODE lle(void const*const e)
{
	LLNODE x;
	x.e=e;
	x.n=NULL;
	return x;
}

void lladd(LLNODE*const list, void const*const e)
{
	if(list==NULL)
	{
		*list=lle(e);
		return;
	}
	LLNODE*x=list;
	while(x->n!=NULL)
	{
		x=x->n;
	}
	x->n=e;
}

LLNODE*llrm(LLNODE*const list, void const*const e)
{
	if(list==NULL)
		return NULL;
	LLNODE*x = list;
	while(x->n!=e)
	{
		if(x->n==NULL)
			return NULL;
		x=x->n;
	}
	LLNODE*r=x->n;
	x->n=x->n->n;
	return r;
}
