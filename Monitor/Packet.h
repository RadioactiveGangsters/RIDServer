#ifndef RPACKET_H
#define RPACKET_H

#include "stdint.h"
#include "sys/types.h"

#include "../Util/DeathRow.h"
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
	#define OPC__MAXOPCODE OPC_VALUE
} opcode;

typedef struct
{
	uint8_t op;
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
	uint32_t unit;
	uint32_t qlen;
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
	int value;
};

struct Update
{
	Packet base;
	uint32_t unit;
	uint32_t sensorlen;
	uint32_t*sensors;
};

struct Alarm
{
	Packet base;
	uint32_t unit;
	uint32_t currentvalue;
	uint32_t counteractiontype;
	uint32_t sensornumber;
};

struct iAlarm
{
	iPacket base;
	char*name;
};

extern Packet*makePing(void);
extern Packet*makeLogin(void);
extern Packet*makeGraph(Sensor const*const);
extern Packet*makeAlarm(Sensor* sn, int actnr);

extern ssize_t writeUpdate(const int, struct Update*);
extern ssize_t writeGraph(const int,struct oGraph*);
extern ssize_t writeAlarm(const int,struct Alarm*);
extern ssize_t writeLogin(const int,struct LoginPacket*);

extern struct iGraph*readGraph(const int);
extern struct iBounds*readBounds(const int);
extern struct iValue*readValue(const int);
extern struct Update*readUpdate(const int);
extern struct iAlarm*readAlarm(const int);

extern iPacket*(*const readOpcode[OPC__MAXOPCODE+1])(const int);

extern void destroyiGraph(struct iGraph*);
extern void destroyoGraph(struct oGraph*);
extern void destroyiBounds(struct iBounds*);
extern void destroyiValue(struct iValue*);
extern void destroyUpdate(struct Update*);
extern void destroyAlarm(struct Alarm*);
extern void destroyLogin(struct LoginPacket*);
extern void destroyiAlarm(struct iAlarm*);

#endif
