#ifndef RPACKET_H
#define RPACKET_H

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <unistd.h>
#endif
#include "../System/Log.h"
#include "../Data/sensor.h"

typedef enum opcode
{
	OPC_UNDEFINED=0x00,
	OPC_LOGIN,
	OPC_PING,
	OPC_UPDATE,
	OPC_GRAPH,
	OPC_ALARM,
	OPC_BOUNDS,
	OPC_VALUE,
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

struct oGraph
{
	Packet base;
	unittype unit;
	int qlen;
	const AutoQ*queue;
};

struct iBounds
{
	Packet base;
	char*name;
	int lbound,ubound;
};

struct iValue
{
	Packet base;
	char*name;
	int value;
};

struct Update
{
	Packet base;
	unittype unit;
	int sensorlen;
	int*sensors;
};

struct Alarm
{
	Packet base;
	unittype unit;
	int currentvalue;
	int counteractiontype;
	int sensornumber;
};

Packet*makePing(void);
Packet*makeLogin(void);
Packet*makeGraph(Sensor const*const);
Packet*makeAlarm(Sensor* sn, int actnr);

ssize_t writeUpdate(const int, struct Update*);
ssize_t writeGraph(const int,struct oGraph*);
ssize_t writeAlarm(const int,struct Alarm*);
ssize_t writeLogin(const int,struct LoginPacket*);

struct iGraph*readGraph(const int);
struct iBounds*readBounds(const int);
struct iValue*readValue(const int);
struct Update*readUpdate(const int);

void destroyiGraph(struct iGraph*);
void destroyoGraph(struct oGraph*);
void destroyiBounds(struct iBounds*);
void destroyiValue(struct iValue*);
void destroyUpdate(struct Update*);
void destroyAlarm(struct Alarm*);
void destroyLogin(struct LoginPacket*);
#endif
