#include "Log.h"

LLNODE*subs;

void subscribe(void(*ls)(const int,char const*const,va_list))
{
	if(!ls)return;
	if(!subs)subs=lle(ls);
	else lladd(subs,ls);
}

void Log(const int ll,char const*const format, ...)
{
	va_list ap;
	LLNODE const*n = subs;
	va_start(ap,format);
	if(n)do
			if(n->e)
				((void(*)(const int,char const*const,va_list))n->e)(ll,format,ap);
		while((n=n->n));
	va_end(ap);
}