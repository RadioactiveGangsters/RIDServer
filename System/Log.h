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
	LOGL_SERIOUS_ERROR,
	LOGL_DEBUG,
	LOGL_SYSTEM_ACTIVITY,
	LOGL_CLIENT_ACTIVITY,
} LOGL;

struct LOGCB
{
	void(*log)(const LOGL,char const*const,va_list);
};

void Log(const LOGL,char const*const, ...);

/*@null@*/void const*
subscribe(void(*)(const LOGL,char const*const,va_list));

#endif