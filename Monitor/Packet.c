#include "Packet.h"

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <unistd.h>
#endif
#include "../System/Log.h"

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
	struct LoginPacket l=
	{
		.base={.op=OPC_LOGIN,},
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
		struct oGraph const g=
		{
			.base={.op=OPC_GRAPH,},
			.unit=unitbystring(s->unit),// FIXME: all units
			.qlen=AutoQcount(s->delta),
			.queue=s->delta,
		};
		struct oGraph*const p=malloc(sizeof*p);
		if(!p) return NULL;
		memcpy(p,&g,sizeof*p);

		if(p->qlen>20){p->qlen=20;}

		return(Packet*)p;
		
	}
}

ssize_t writeLogin(const int fd, struct LoginPacket*p)
{
	if(!fd)return -1;
	if(!p)return -1;
	if(p->base.op!=OPC_LOGIN)return -1;
	if( write(fd,&p->base.op,sizeof(opcode)) == -1 ) return -1;
	if( write(fd,&p->zero,sizeof(int)) == -1 ) return -1;
	return (ssize_t)sizeof(struct LoginPacket);
}

ssize_t writeUpdate(const int fd, struct Update*packet)
{
	if(!fd)return -1;
	if(!packet) return -1;
	if(packet->base.op==OPC_UNDEFINED)return -1;
	if( write(fd,&packet->base.op,sizeof(opcode)) == -1 ) return -1;
	if( write(fd,&packet->unit,sizeof(int)) == -1 ) return -1;
	if( write(fd,&packet->sensorlen,sizeof(int)) == -1 ) return -1;
	if( write(fd,packet->sensors,sizeof(int)*packet->sensorlen) == -1 ) return -1;

	// TODO: recalculate size
	return (ssize_t)sizeof*packet;
}

ssize_t writeGraph(const int fd,struct oGraph*packet)
{
	AutoQ const*e;
	int i=0;
	if(!fd)return -1;
	if(!packet)return -1;
	if(packet->base.op==OPC_UNDEFINED)return -1;

	if( write(fd,&packet->base.op,sizeof(opcode)) == -1 ) return -1;
	if( write(fd,&packet->unit,sizeof(int)) == -1 ) return -1;
	//if( write(fd,&packet->name,sizeof(char)*packet->namelen) == -1 ) return -1;
	if( write(fd,&packet->qlen,sizeof(int)) == -1 ) return -1;
	
	e=packet->queue;
	while(e && i++<packet->qlen);
	{
		// TODO: binary sensors?
		if( write(fd,e->e,sizeof(int)) == -1 ) return -1;
		e=e->n;
	}

	return (ssize_t)sizeof(struct oGraph);
}

struct iGraph*readGraph(const int source)
{
	struct iGraph u=
	{
		.base={.op=OPC_UNDEFINED,},
		.name=NULL,
	},*p=malloc(sizeof(struct iGraph));

	uint32_t requestsize=0;
	size_t wanted;
	ssize_t expected;
	char*sensor;
	unsigned int i;

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
	if(requestsize<1)
	{
		return p;
	}

	wanted=(sizeof(char)*requestsize);
	expected=(ssize_t)wanted;
	sensor=malloc(wanted+sizeof(char));
	if(!sensor)return NULL;
	for(i=(unsigned int)wanted+1;--i;)
	{
		sensor[i]=0;
	}
	if(recv(source, sensor, wanted,MSG_WAITALL)!=expected)
	{
		return p;
	}

	p->base.op=OPC_GRAPH;
	p->name=sensor;
	return p;
}

struct iBounds*readBounds(const int source)
{
	struct iBounds b=
	{
		.base={.op=OPC_UNDEFINED,},
		.name=NULL,
		.lbound=INT_MIN,
		.ubound=INT_MAX,
	},*p=malloc(sizeof*p);
	size_t wanted;
	uint32_t namelen,lbound,ubound;
	char*sensor;

	if(!p)return NULL;
	memcpy(p,&b,sizeof*p);

	wanted=sizeof(int);
	if(recv(source,&namelen,wanted,MSG_WAITALL)==-1)
	{
		return p;
	}
	namelen=ntohl(namelen);

	if(namelen<1)
	{
		return p;
	}

	wanted=sizeof(char)*namelen;
	sensor=malloc(wanted+sizeof(char));
	if(!sensor)
	{
		free(p);
		return NULL;
	}
	{unsigned int i;for(i=(unsigned int)wanted+1;--i;)
	{
		sensor[i]=0;
	}}

	if(recv(source,sensor,wanted,MSG_WAITALL)==-1)
	{
		return p;
	}

	wanted=sizeof(int);
	if(recv(source,&lbound,wanted,MSG_WAITALL)==-1)
	{
		return p;
	}
	lbound=ntohl(lbound);
	if(recv(source,&ubound,wanted,MSG_WAITALL)==-1)
	{
		return p;
	}
	ubound=ntohl(ubound);

	p->base.op=OPC_BOUNDS;
	p->name=sensor;
	p->lbound=(int)lbound;
	p->ubound=(int)ubound;
	return p;
}

struct iValue*readValue(const int source)
{
	struct iValue v=
	{
		.base={.op=OPC_UNDEFINED,},
		.name=NULL,
		.value=INT_MIN,
	},*p=malloc(sizeof*p);
	size_t wanted;
	uint32_t namelen,value;
	char*sensor;

	if(!p)return NULL;
	memcpy(p,&v,sizeof*p);

	wanted=sizeof(int);
	if(recv(source,&namelen,wanted,MSG_WAITALL)==-1)
	{
		return p;
	}
	namelen=ntohl(namelen);

	if(namelen<1)
	{
		return p;
	}

	wanted=sizeof(char)*namelen;
	sensor=malloc(wanted+sizeof(char));
	if(!sensor)
	{
		free(p);
		return NULL;
	}
	{unsigned int i;for(i=(unsigned int)wanted+1;--i;)
	{
		sensor[i]=0;
	}}

	if(recv(source,sensor,wanted,MSG_WAITALL)==-1)
	{
		return p;
	}

	wanted=sizeof(int);
	if(recv(source,&value,wanted,MSG_WAITALL)==-1)
	{
		return p;
	}
	value=ntohl(value);

	p->base.op=OPC_VALUE;
	p->name=sensor;
	p->value=(int)value;
	return p;
}


void destroyLogin(struct LoginPacket*l)
{
	free(l);
}

void destroyUpdate(struct Update*u)
{
	free(u->sensors);
	u->sensors=NULL;
	free(u);
	u=NULL;
}

void destroyiGraph(struct iGraph*g)
{
	free(g->name);
	g->name=NULL;
	free(g);
	g=NULL;
}

void destroyoGraph(struct oGraph*g)
{
	free(g);
	g=NULL;
}

void destroyiBounds(struct iBounds*b)
{
	free(b->name);
	b->name=NULL;
	free(b);
	b=NULL;
}

void destroyiValue(struct iValue*v)
{
	free(v->name);
	v->name=NULL;
	free(v);
	v=NULL;
}

