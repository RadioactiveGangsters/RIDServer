#include "Client.h"

void _writePacket(const int fd,Packet*const p)
{
	if(!p)return;
	else
	{
		ssize_t packsize;
		switch(p->op)
		{
			case OPC_ALARM:
			case OPC_UPDATE:
			{
				struct Update*u=(struct Update*)p;
				Log(LOGT_CLIENT,LOGL_DEBUG,"writeUpdate: %d",packsize=writeUpdate(fd,u));
				// TODO:destroyUpdate(u)
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
				break;
			case OPC_PING:
				Log( LOGT_CLIENT,LOGL_DEBUG,"writePing: %d",write(fd,&p->op,sizeof(char)));
				free(p);
				break;
		}

	}
}

static void fillarray(Trie const*const table,int*array,unsigned int*i)
{
	if(!table)return;
	if(!array)return;
	if(!i)return;
	fillarray(table->l,array,i);
	fillarray(table->g,array,i);
	array[*i]=((iSensor*)table->e)->value;
	*i=*i+1;
}

static void sendupdates(Trie*const table, Client*const client)
{
	if(!table)return;
	{
		struct Update u=
		{
			.base={.op=OPC_UNDEFINED,},
			.unit=unit_temperature,
			.sensorlen=triecount(table),
			.sensors=NULL,
		},*p=malloc(sizeof*p);

		if(!p)return;
		memcpy(p,&u,sizeof*p);
		{
			int*sensorarray=malloc(sizeof(int)*p->sensorlen);
			unsigned int i=0;
			if(!p)
			{
				free(p);
				return;
			}
			fillarray(table,sensorarray,&i);
			p->sensors=sensorarray;
			p->base.op=OPC_UPDATE;
			sendPacket(client,(Packet*)p);
		}
	}
}

void passclient(Trie*const table,Client*const client,void(*cb)(Trie*const,Client*const))
{
	if(!table)return;
	if(!client)return;
	if(!cb)return;
	passclient(table->l,client,cb);
	passclient(table->g,client,cb);
	cb(table,client);
}

void*_iLoop(void*const c)
{
	if(!c)
	{
		pthread_exit(NULL);
	}
	else
	{
		Client*client=c;
		Log(LOGT_NETWORK,LOGL_DEBUG, "waiting for input");
		while(true)
		{
			opcode ch=OPC_UNDEFINED;
			if(read(client->fd, &ch, 1)!=1)
			{
				Log(LOGT_NETWORK,LOGL_CLIENT_ACTIVITY,"client disconnected");
				// TODO: cleanup
				break;
			}
			else
			{
				Packet*p;
				Sensor const* s;
				Log(LOGT_NETWORK,LOGL_DEBUG, "Client send: %c (%d)", ch, ch);
				switch(ch)
				{
					case OPC_LOGIN:
						Log(LOGT_NETWORK,LOGL_WARNING,"A superior client tried to log in, this server version does not yet support client initialisation.");
						p=makeLogin();
						sendPacket(client,p);
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
						destroyiGraph(ig);
						
						sendPacket(client,p);
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
							break;
						}
						if(ib->base.op==OPC_UNDEFINED)
						{
							Log(LOGT_CLIENT,LOGL_BUG,"Cannot read packet");
							continue;
						}
						Log(LOGT_CLIENT,LOGL_DEBUG,"read bonuds packet concerning %s (%d,%d)",ib->name,ib->lbound,ib->ubound);
						s= findSensor(ib->name);
						if(!s)
						{
							Log(LOGT_CLIENT,LOGL_BUG,"but that sensor does not exist");
						}
						// TODO: setbounds
					}
					case OPC_UPDATE:
					{
						Trie*const db=Tables();
						passclient(db,client,&sendupdates);
						break;
					}
					case OPC_ALARM:
					case OPC_UNDEFINED:
						Log(LOGT_NETWORK,LOGL_ERROR,"Client violates protocol");
						goto dead;
						break;
				}
			}
			sleep(1);
		}
		dead:;
	}
	pthread_exit(NULL);
}

void*_oLoop(void*const c)
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
			while(client->_queue)
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

void test(void)
{
	int i = 0;
	//printf("Received: %c\n", ch);

	//ch++;

	//if(sleep(5)) break;

	for(; i < 20; i++)
	{
//		ssize_t x;
//		if((x = write(address, pp, sizeof(char))) != sizeof(char))
//		Log(LOGT_NETWORK,LOGL_ERROR, "Cannot send %d: %d",x, errno);

//		Log(LOGT_NETWORK,LOGL_DEBUG, "Send char %d ", *pp);


		//Log(LOGL_CLIENT_ACTIVITY, "send char %c ", t );
	}

//	Log(LOGT_NETWORK,LOGL_DEBUG, "Server send: %c", ch);
//	if(write(address, &ch, 1)) continue;
//	printf("Send: %c", ch);

//	(void)close(address);
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

