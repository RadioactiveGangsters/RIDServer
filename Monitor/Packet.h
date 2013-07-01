#ifndef RPACKET_H
#define RPACKET_H

#include <stdlib.h>
#include <string.h>

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

struct LoginPacket
{
	Packet base;
	int zero;
};

Packet*makePing(void);
Packet*makeLogin(void);

#endif
