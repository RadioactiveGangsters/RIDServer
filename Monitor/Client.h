#ifndef CLIENT_H
#define CLIENT_H

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

#include <pthread.h>

#include "Packet.h"
#include "../System/Log.h"

typedef struct
{
	const int fd,
	const pthread_t iloop, oloop;
	const void *send(Packet*),
} Client;

Client* MakeClient(int);

void sendPacket(Packet*);

#endif