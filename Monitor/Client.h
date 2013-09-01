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
#include "../Data/Database.h"
#include "../Util/queue.h"

typedef struct Client Client;

struct Client
{
	const int fd;
	pthread_t iloop, oloop;
	void(*const relinquish_monitor)(Client*const);
	queue*_queue;
};

Client*SpawnClient(const int,void(*const relinquish_monitor)(Client*const));

void sendPacket(Client*const,Packet*const);

#endif
