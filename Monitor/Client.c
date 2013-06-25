#include "Client.h"


Client* MakeClient(int fd)
{
	pthread_t iloop,oloop;
	Client c =
	{
		.fd=fd,
		.send=&sendpacket,
		.iloop=iloop,
		.oloop=oloop,
	},*p=malloc(sizeof*p);
	if(!p)
	{
		Log(LOGL_SERIOUS_ERROR,LOGT_NETWORK,"Out of memory!");
		return NULL;
	}
	memcpy(p,&c,sizeof*p);
	pthread_create(&p->iloop,NULL,&iLoop,p);
	pthread_create(&p->oloop,NULL,$oLoop,p);
	return p;
}

void test(void);
}
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

		//if((x=write(client_sockfd, t, sizeof(char)))!=sizeof(char))
		//Log(LOGL_ERROR, "string cannot send %d: %d",x, errno);

		//Log(LOGL_CLIENT_ACTIVITY, "send char %c \n", t );
	}

//	Log(LOGL_DEBUG, LOGT_NETWORK, "Server send: %c\n", ch);
//	if(write(address, &ch, 1)) continue;
//	printf("Send: %c", ch);

	(void)close(address);
}


static void iLoop(Client*c)
{

}

static void oLoop(Client*c)
{
	while(true)
	{
		if(Client->queue)
		{
			
		}
	}
}

void sendPacket(Packet*)
{

}