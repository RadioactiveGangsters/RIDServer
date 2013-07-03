#ifndef FRONTEND_H
#define	FRONTEND_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#include "../System/Log.h"
#include "../System/Server.h"
#include "../System/Alarm.h" 

#ifdef _WIN32

	#include <windows.h>
	#define COLOURSUPPORT true

#else

	#define COLOURSUPPORT true

#endif

static bool useColours=COLOURSUPPORT;

int main(int,char**);

static void console(const LOGL,const LOGT,char const*const,va_list);

#endif
