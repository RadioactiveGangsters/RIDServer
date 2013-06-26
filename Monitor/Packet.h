#ifndef RPACKET_H
#define RPACKET_H

typedef enum opcode
{
	OPC_UNDEFINED=0x00,
	OPC_PING=0x02,
} opcode;

typedef struct
{
	opcode op;
} Packet;

#endif