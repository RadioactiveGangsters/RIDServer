#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include "Log.h"
#include "Alarm.h"
#include "../Data/Database.h"
#include "../Data/ReactorData.h"
#include "../Monitor/Printer.h"
#include "../Monitor/connection.h"

int InitServer(const int, char const*const*const);
int StartServer(void);

#endif
