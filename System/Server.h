#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include "connection.h"
#include "Log.h"

int InitServer(const int, char const*const*const);
int StartServer(void);

#endif