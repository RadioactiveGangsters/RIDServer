#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include "../Monitor/connection.h"
#include "Log.h"
#include "../Data/Database.h"
#include "../Data/ReactorData.h"

int InitServer(const int, char const*const*const);
int StartServer(void);

#endif
