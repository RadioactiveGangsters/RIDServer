#include "Client.h"

void*_iLoop(void*const c)
{
	if(!c)
	{
		pthread_exit(NULL);
	}
	else
	{
//		Client*client=c;
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

Client*SpawnClient(int fd)
{
	pthread_t iloop,oloop;
	Client c=
	{
		.fd=fd,
		.iloop=iloop,
		.oloop=oloop,
		._queue=NULL,
		.send=&sendPacket,
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
	//if(read(address, &ch, 1)) break; 
	//Log(LOGL_DEBUG, LOGT_NETWORK, "Client send: %c\n", ch);
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


void sendPacket(Packet*p)
{
	if(!p)return;
}