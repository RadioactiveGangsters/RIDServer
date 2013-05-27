#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdio.h>

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

void Log(const LOGL ll,char const*const, ...);
void subscribe(void(*ls)(const LOGL,char const*const,va_list));

#endif