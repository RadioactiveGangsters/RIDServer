#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdio.h>

#include "../Util/LinkedList.h"

void Log(const int ll,char const*const, ...);
void subscribe(void(*ls)(const int,char const*const,va_list));

#endif