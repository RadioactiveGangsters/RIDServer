#ifndef FRONTEND_H
#define	FRONTEND_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "../System/Log.h"
#include "../System/Server.h"

static bool useColours=true;

int main(int,char**);

static void console(const LOGL,char const*const,va_list);

#endif
