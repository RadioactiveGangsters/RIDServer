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

#include "../System/Log.h"

int socklisten(void);

#endif
