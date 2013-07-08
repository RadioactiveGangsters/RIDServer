#include "Client.h"

void _writePacket(const int fd,Packet*const p)
{
	if(!p)return;
	else
	{
		size_t packsize;
		switch(p->op)
		{
			case OPC_ALARM:
			case OPC_UPDATE:
			case OPC_GRAPH:
			case OPC_BOUNDS:
			case OPC_UNDEFINED:
				return;
			case OPC_LOGIN:
				packsize=sizeof(struct LoginPacket);
				break;
			case OPC_PING:
				packsize=sizeof(Packet);
				break;
		}

		if(write(fd,p,packsize)<(ssize_t)packsize)
		{
			Log(LOGT_NETWORK,LOGL_WARNING, "Could not send complete packet %d",p->op);
			return;
		}
	}
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
						Log(LOGT_NETWORK,LOGL_BUG,"packet %d not supported yet",ch);
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
		Client const*const client=c;
		while(true)
		{
			sleep(1);
			if(client->_queue)
			{

				Packet const*const p=dequeue(client->_queue);
				if(!p||p->op==OPC_UNDEFINED)continue;
				Log(LOGT_CLIENT,LOGL_DEBUG,"writing out packet %d: %d",p->op,writeGraph(client->fd,c));
				destroyoGraph((struct oGraph*)p);
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

