#include "Client.h"

void writePacket(const int fd,Packet*const p)
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
			Log(LOGL_WARNING, LOGT_NETWORK, "Could not send complete packet %d\n",p->op);
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
		Log(LOGL_DEBUG, LOGT_NETWORK, "waiting for input\n");
		while(true)
		{
			opcode ch;
			if(read(client->fd, &ch, 1)!=1)
			{
				Log(LOGL_CLIENT_ACTIVITY,LOGT_NETWORK,"client read error\n");
				break;
			}
			else
			{
				Packet*p;
				Log(LOGL_DEBUG, LOGT_NETWORK, "Client send: %c (%d)\n", ch, ch);
				switch(ch)
				{
					case OPC_LOGIN:
						Log(LOGL_WARNING,LOGT_NETWORK,"A superior client tried to log in, this server version does not yet support client initialisation.\n");
						p=makeLogin();
						sendPacket(client,p);
						break;

					case OPC_PING:
						// Do not cause pingstorm
						break;

					case OPC_UPDATE:
					case OPC_GRAPH:
						Log(LOGL_BUG,LOGT_NETWORK,"packet %d not supported yet\n",ch);
						continue;
					case OPC_ALARM:
					case OPC_UNDEFINED:
						Log(LOGL_ERROR,LOGT_NETWORK,"Client violates protocol\n");
						goto dead;
						break;
				}
			}
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
		Client*client=c;
		while(true)
		{
			if(client->_queue)
			{
//				ssize_t x;
//				if((x=write(client->fd, dequeue(client->_queue), sizeof(char)))!=sizeof(char))
				{
					// TODO: assume client is dead; cleanup
					break;
				}
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
//		Log(LOGL_ERROR, LOGT_NETWORK, "Cannot send %d: %d\n",x, errno);

//		Log(LOGL_DEBUG, LOGT_NETWORK, "Send char %d \n", *pp);


		//Log(LOGL_CLIENT_ACTIVITY, "send char %c \n", t );
	}

//	Log(LOGL_DEBUG, LOGT_NETWORK, "Server send: %c\n", ch);
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
		//enqueue(c,p);
	}
}

