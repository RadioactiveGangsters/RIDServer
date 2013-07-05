#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "../Util/LinkedList.h"

typedef enum
{
	LOGL_UNDEFINED,
	LOGL_ERROR,
	LOGL_WARNING,
	LOGL_BUG,
	LOGL_SERIOUS_ERROR,
	LOGL_ALARM,
	LOGL_DEBUG,
	LOGL_SYSTEM_ACTIVITY,
	LOGL_CLIENT_ACTIVITY,
} LOGL;

typedef enum
{
	LOGT_UNDEFINED,
	LOGT_PROGRAM,
	LOGT_SERVER,
	LOGT_DB,
	LOGT_PRINTER,
	LOGT_NETWORK,
	LOGT_CLIENT,
} LOGT;

struct LOGCB
{
	void(*log)(const LOGT,const LOGL,char const*const,va_list);
};

void Log(const LOGT,const LOGL,char const*const, ...);

/*@null@*/void const*
subscribe(void(*)(const LOGT,const LOGL,char const*const,va_list));

#endif
