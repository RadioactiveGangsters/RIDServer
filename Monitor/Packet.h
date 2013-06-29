#ifndef RPACKET_H
#define RPACKET_H

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../System/Log.h"

typedef enum opcode
{
	OPC_UNDEFINED=0x00,
	OPC_LOGIN,
	OPC_PING,
	OPC_UPDATE,
	OPC_GRAPH,
	OPC_ALARM,
} opcode;

typedef struct
{
	opcode op;
} Packet;

typedef Packet iPacket;

struct LoginPacket
{
	Packet base;
	int zero;
};

struct iGraph
{
	iPacket base;
	char*name;
};

Packet*makePing(void);
Packet*makeLogin(void);

iPacket*readGraph(int);

#endif
