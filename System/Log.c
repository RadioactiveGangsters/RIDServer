#include "Log.h"

static LLNODE*subs;

void const*subscribe(void(*ls)(const LOGL,char const*const,va_list))
{
	if(!ls)return NULL;
	{
		struct LOGCB cb=
		{
			.log=ls,
		},*p=malloc(sizeof*p);

		if(!p)return NULL;
		memcpy(p,&cb,sizeof*p);

		if(!subs)
		{
			subs=lle(p);
		}
		else
		{
			lladd(subs,p);
		}
		return p;
	}
}

void Log(const LOGL ll,char const*const format, ...)
{
	va_list ap;
	LLNODE const*n = subs;
	va_start(ap,format);
	if(n)do
			if(n->e)
				((struct LOGCB*)n->e)->log(ll,format,ap);
		while((n=n->n));
	va_end(ap);
}