#ifndef RCLIENT_H
#define RCLIENT_H

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
#include <stdbool.h>

#include "Packet.h"
#include "../System/Log.h"

typedef struct
{
	const int fd;
	pthread_t iloop, oloop;
	void*_queue;
	void (*const send)(Packet*const);
} Client;

Client*SpawnClient(const int);

void sendPacket(Packet*const);

#endif
