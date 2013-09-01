#include "Client.h"

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
#include "../System/Log.h"
#include "../Data/Database.h"

static void _writePacket(const int fd,Packet*const p)
{
	if(!p)return;
	else
	{
		ssize_t packsize;
		switch(p->op)
		{
			case OPC_ALARM:
			{
				struct Alarm*a=(struct Alarm*)p;
				Log(LOGT_CLIENT,LOGL_DEBUG,"writeAlarm for %d: %d",ntohl(a->unit),packsize=writeAlarm(fd,a));
				destroyAlarm(a);
				break;
			}
			case OPC_UPDATE:
			{
				struct Update*u=(struct Update*)p;
				Log(LOGT_CLIENT,LOGL_DEBUG,"writeUpdate for %d: %d",ntohl(u->unit),packsize=writeUpdate(fd,u));
				destroyUpdate(u);
				break;
			}
			case OPC_GRAPH:
			{
				struct oGraph*g=(struct oGraph*)p;
				Log(LOGT_CLIENT,LOGL_DEBUG,"writegraph: %d",packsize=writeGraph(fd,g));
				destroyoGraph(g);
				break;
			}
			case OPC_BOUNDS:
				// TODO
			case OPC_UNDEFINED:
				return;
			case OPC_LOGIN:
			{
				struct LoginPacket*l=(struct LoginPacket*)p; 
				Log(LOGT_CLIENT,LOGL_DEBUG,"writeLogin: %d",writeLogin(fd,l));
				destroyLogin(l);
				break;
			}
			case OPC_PING:
				Log( LOGT_CLIENT,LOGL_DEBUG,"writePing: %d",write(fd,&p->op,sizeof(char)));
				free(p);
				break;
		}

	}
}

static void fillarray(Trie const*const table,uint32_t*array,unsigned int*i)
{
	uint32_t v=0;
	if(!table)return;
	if(!array)return;
	if(!i)return;
	fillarray(table->l,array,i);
	fillarray(table->g,array,i);
	switch(((Sensor*)table->e)->type)
	{
		case integersensor:
		{
			iSensor*s=table->e;
			v=s->value;
			break;
		}
		case binarysensor:
		{
			bSensor*s=table->e;
			v=s->value?1:0;
			break;
		}
	}
	array[*i]=ntohl(v);
	*i=*i+1;
}

static void sendupdates(Trie*const table, Client*const client)
{
	if(!table)return;
	{
		struct Update u=
		{
			.base={.op=OPC_UNDEFINED,},
			.unit=htonl(unitbystring(table->id)),
			.sensorlen=htonl(triecount(table->e)),
			.sensors=NULL,
		},*p=malloc(sizeof*p);

		if(!p)return;
		memcpy(p,&u,sizeof*p);
		{
			uint32_t*sensorarray=malloc(sizeof(uint32_t)*ntohl(p->sensorlen));
			unsigned int i=0;
			if(!p)
			{
				free(p);
				return;
			}
			fillarray(table->e,sensorarray,&i);
			p->sensors=sensorarray;
			p->base.op=OPC_UPDATE;
			sendPacket(client,(Packet*)p);
		}
	}
}

static void passclient(Trie*const table,Client*const client,void(*cb)(Trie*const,Client*const))
{
	if(!table)return;
	if(!client)return;
	if(!cb)return;
	passclient(table->l,client,cb);
	passclient(table->g,client,cb);
	cb(table,client);
}

static void setFlow(Trie*const sensorbox,void*rawvalue)
{
	((iSensor*)sensorbox)->value=*(int*)rawvalue;
	Log(LOGT_CLIENT,LOGL_DEBUG,"set a flow");
}

static void*_iLoop(void*const c)
{
	if(!c)
	{
		pthread_exit(NULL);
	}
	else
	{
		bool debug=false,connected=true;
		Client*client=c;
		Log(LOGT_NETWORK,LOGL_DEBUG, "waiting for input");
		while(connected)
		{
			opcode ch=OPC_UNDEFINED;
			if(read(client->fd, &ch, 1)!=1)
			{
				Log(LOGT_NETWORK,LOGL_CLIENT_ACTIVITY,"client disconnected");
				break;
			}
			else
			{
				Packet*p;
				Sensor const* s;
				if(debug)ch-=48;
				Log(LOGT_NETWORK,LOGL_DEBUG, "Client send: %c (%d)", ch, ch);
				switch(ch)
				{
					case OPC_LOGIN:
						Log(LOGT_NETWORK,LOGL_WARNING,"A superior client tried to log in, this server version does not yet support client initialisation.");
						p=makeLogin();
						sendPacket(client,p);
						debug=true;
						break;

					case OPC_PING:
						// Do not cause pingstorm
						break;

					case OPC_GRAPH:
					{
						struct iGraph*ig;
						Log(LOGT_CLIENT,LOGL_DEBUG,"reading graph packet");
						ig = readGraph(client->fd);
						if(!ig)
						{
							Log(LOGT_CLIENT,LOGL_SERIOUS_ERROR,"Out of memory!");
							connected=false;
							break;
						}
						if(ig->base.op==OPC_UNDEFINED)
						{
							Log(LOGT_CLIENT,LOGL_BUG,"Cannot read packet");
							continue;
						}
						Log(LOGT_CLIENT,LOGL_DEBUG,"read graph packet requesting sensor %s.",ig->name);
						
						s = findSensor(ig->name);
						p=makeGraph(s);
						if(!p)
						{
							Log(LOGT_CLIENT,LOGL_BUG,"requested sensor %s invalid.",ig->name);
							break;
						}
						
						sendPacket(client,p);
						destroyiGraph(ig);
						break;
					}
					case OPC_ALARM:
					{
						struct iAlarm*ia;
						Log(LOGT_CLIENT,LOGL_DEBUG,"Reading Alarm response");
						ia=readAlarm(client->fd);
						if(ia==NULL)
						{
							Log(LOGT_CLIENT,LOGL_SERIOUS_ERROR,"Out of memory!");
							connected=false;
							break;
						}
						if(ia->base.op==OPC_UNDEFINED)
						{
							Log(LOGT_CLIENT,LOGL_BUG,"Cannot read packet");
							continue;
						}
						Log(LOGT_CLIENT,LOGL_DEBUG,"read alarm response requesting sensor %s.",ia->name);

						s=findSensor(ia->name);
						if(s==NULL)
						{
							Log(LOGT_CLIENT,LOGL_BUG,"requested sensor %s invalid.",ia->name);
							break;
						}

						ResetSensor(s);
						break;
					}
					case OPC_BOUNDS:
					{
						struct iBounds*ib;
						Log(LOGT_CLIENT,LOGL_CLIENT_ACTIVITY,"bounds request");
						ib=readBounds(client->fd);

						if(!ib)
						{
							Log(LOGT_CLIENT,LOGL_SERIOUS_ERROR,"Out of memory!");
							connected=false;
							break;
						}
						if(ib->base.op==OPC_UNDEFINED)
						{
							Log(LOGT_CLIENT,LOGL_BUG,"Cannot read packet");
							continue;
						}
						Log(LOGT_CLIENT,LOGL_DEBUG,"read bounds packet concerning %s (%d,%d)",ib->name,ib->lbound,ib->ubound);
						s= findSensor(ib->name);
						if(!s)
						{
							Log(LOGT_CLIENT,LOGL_BUG,"but that sensor does not exist");
							continue;
						}
						
						if(!s->type==binarysensor)
						{
							if(!(ib->lbound < 1))
							{
								((iSensor*)s)->lbound = ib->lbound;
								((iSensor*)s)->ubound = ib->ubound;
							} else {
								Log(LOGT_CLIENT,LOGL_CLIENT_ACTIVITY,"LOWERBOUND seems to be invalid");
								((iSensor*)s)->ubound = ib->ubound;
							}
							Log(LOGT_CLIENT,LOGL_DEBUG,"%s bounds have been changed(LOWERBOUND: %d,UPPERBOUND: %d)",s->name,((iSensor*)s)->lbound,((iSensor*)s)->ubound);
							destroyiBounds(ib);
							break;
						}
						
					}
					case OPC_VALUE:
					{
						struct iValue*iv;
						Log(LOGT_CLIENT,LOGL_CLIENT_ACTIVITY,"value request");
						iv = readValue(client->fd);

						if(!iv)
						{
							Log(LOGT_CLIENT,LOGL_SERIOUS_ERROR,"Out of memory!");
							connected=false;
							break;
						}
						if(iv->base.op==OPC_UNDEFINED)
						{
							Log(LOGT_CLIENT,LOGL_BUG,"Cannot read packet");
							continue;
						}
						Log(LOGT_CLIENT,LOGL_DEBUG,"read value packet: %d",iv->value);
						if(!(iv->value < 1))
						{
							xfortrie(findinTrie(Tables(),"Flow"),&setFlow,&iv->value);
							((iSensor*)s)->value = iv->value;
							Log(LOGT_CLIENT,LOGL_DEBUG,"flow value have been changed(VALUE: %d)",((iSensor*)s)->value);
						} 
						else Log(LOGT_CLIENT,LOGL_CLIENT_ACTIVITY,"VALUE seems to be invalid");
						destroyiValue(iv);
						break;
						
					}
					case OPC_UPDATE:
					{
						Trie*const db=Tables();
						passclient(db,client,&sendupdates);
						break;
					}
					case OPC_UNDEFINED:
						Log(LOGT_NETWORK,LOGL_ERROR,"Client violates protocol");
						connected=false;
						break;
				}
			}
			sleep(1);
		}
		dead:;
		// TODO: clear queue
		shutdown(client->fd,SHUT_RDRW);

	}
	pthread_exit(NULL);
}

static void*_oLoop(void*const c)
{
	if(!c)
	{
		pthread_exit(NULL);
	}
	else
	{
		Client *const client=c;
		unsigned int pingcounter=80;
		while(true)
		{
			Rest(1);
			if(!pingcounter--)
			{
				sendPacket(client,makePing());
			}
			if(client->_queue)while(client->_queue->count)
			{
				Packet*const p=dequeue(client->_queue);
				if(!p||p->op==OPC_UNDEFINED)continue;
				_writePacket(client->fd,p);
			}
		}
	}
	pthread_exit(NULL);
}

Client*SpawnClient(const int fd)
{
	pthread_t iloop=0,oloop=0;
	Client c=
	{
		.fd=fd,
		.iloop=iloop,
		.oloop=oloop,
		._queue=NULL,
	},*p=malloc(sizeof*p);
	if(!p)return NULL;

	memcpy(p,&c,sizeof*p);
	pthread_create(&p->iloop,NULL,&_iLoop,p);
	pthread_create(&p->oloop,NULL,&_oLoop,p);
	return p;
}

void sendPacket(Client*const c,Packet*const p)
{
	if(!c)return;
	if(!p)return;
	else
	{
		if(p->op==OPC_UNDEFINED)return;
		if(!c->_queue)
		{
			c->_queue=malloc(sizeof(queue));
			if(!c->_queue)return;
			init_queue(c->_queue);
		}
		enqueue(c->_queue,p);
	}
}

