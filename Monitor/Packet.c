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

iPacket*readGraph(const int source)
{
	iPacket u=
	{
		.op=OPC_UNDEFINED,
	},g=
	{
		.op=OPC_GRAPH,
	},*p=malloc(sizeof(struct iGraph));

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
	requestsize=ntohl(requestsize);

	wanted=((sizeof(char)*requestsize)*2)+1;
	expected=(ssize_t)wanted;
	sensor=malloc(wanted);
	if(!sensor)return NULL;
	if(recv(source, sensor, wanted,MSG_WAITALL)!=expected)
	{
		return p;
	}
	sensor[requestsize]='\0';
	{
		uint32_t i=requestsize<<1;
		do
		{
			sensor[i]|='.';
		}
		while(i-=2);
		sensor[i]|='.';
	}

	*p=*(iPacket*)(struct iGraph[]){{
		.base=g,
		.name=sensor
	}};
	return p;
}

