#include "Packet.h"

Packet*makePing(void)
{
	Packet b=
	{
		.op=OPC_PING,
	},*p=malloc(sizeof*p);
	if(!p)return NULL;
	memcpy(p,&b,sizeof*p);
	return p;
}

Packet*makeLogin(void)
{
	Packet b=
	{
		.op=OPC_LOGIN,
	};
	struct LoginPacket l=
	{
		.base=b,
		.zero=0,
	},*p=malloc(sizeof*p);
	if(!p)return NULL;
	memcpy(p,&l,sizeof*p);
	return (Packet*)p;
}

Packet*makeGraph(Sensor const*const s)
{
	if(!s||!s->delta)
	{
		return NULL;
	}
	{
		Packet const b={.op=OPC_LOGIN,};
		struct oGraph const g=
		{
			.base=b,
			.namelen=SENSOR_HNAMELEN,
			.name=s->name,
			.qlen=AutoQcount(s->delta),
			.queue=s->delta,
		};
		struct oGraph*const p=malloc(sizeof*p);
		if(!p) return NULL;
		memcpy(p,&g,sizeof*p);

		return(Packet*)p;
		
	}
}

iPacket*readGraph(const int source)
{
	iPacket u=
	{
		.op=OPC_UNDEFINED,
	},*p=malloc(sizeof(struct iGraph));
	struct iGraph*g=(struct iGraph*)p;

	uint32_t requestsize=0;
	size_t wanted;
	ssize_t expected;
	char*sensor;

	if(!p)return NULL;
	memcpy(p,&u,sizeof*p);

	wanted=sizeof(int);
	expected=(ssize_t)wanted;
	if(recv(source, &requestsize, wanted,MSG_WAITALL)!=expected)
	{
		return p;
	}
	// only needed for int sizes
	requestsize=ntohl(requestsize);
	if(!requestsize)
	{
		return p;
	}

	wanted=(sizeof(char)*requestsize);
	expected=(ssize_t)wanted;
	sensor=malloc(wanted+sizeof(char));
	if(!sensor)return NULL;
	if(recv(source, sensor, wanted,MSG_WAITALL)!=expected)
	{
		return p;
	}
	printf("%s\n",sensor);

	g->base.op=OPC_GRAPH;
	g->name=sensor;
	return p;
}

