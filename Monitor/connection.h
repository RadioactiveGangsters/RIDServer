#ifndef RIDSCONN_H
#define RIDSCONN_H

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32

	#include <windows.h>

#else

	#include <unistd.h>
	#include <signal.h>
	#include <sys/un.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>

#endif

#include <errno.h>

#include "Client.h"
#include "../System/Log.h"
#include "../Util/Path.h"
#include "../deps/iniparser/src/iniparser.h"
#include "../System/Alarm.h"

int AcceptClients(void);

void forClients(void(*)(Client*const,void*),void*);

#endif
