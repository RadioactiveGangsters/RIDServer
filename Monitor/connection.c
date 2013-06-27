#include "connection.h"

void*socklisten(void*connection)
{
	if(!connection)
	{
		Log(LOGL_BUG,LOGT_NETWORK,"Cannot listen without connection");
		pthread_exit(NULL);
	}
	else
	{
		int server_sockfd = *(int*)connection;
	    Log(LOGL_SYSTEM_ACTIVITY, LOGT_NETWORK, "Configuring complete, ready for Client connections.\n");
		while(1)
		{
			struct sockaddr client_address;
			socklen_t clien_len = (socklen_t)sizeof(client_address);
			int client_sockfd;
			Client*c;

			if((client_sockfd = accept(server_sockfd, &client_address, &clien_len)) < 1 )
			{
				Log(LOGL_BUG,LOGT_NETWORK,"Unable to accept connection, dying.");
				break;
			}
			Log(LOGL_CLIENT_ACTIVITY, LOGT_NETWORK, "Client connected\n");       

			c=SpawnClient(client_sockfd);
			if(!c)
			{
				Log(LOGL_SERIOUS_ERROR,LOGT_NETWORK,"Out of memory!");
				pthread_exit(NULL);
			}
		}
	}
	free(connection);
	pthread_exit(NULL);
}

int AcceptClients(void)
{
	#ifdef _WIN32
	Sleep(100000000);
	#else
	pthread_t netthread;
	int server_sockfd;
	dictionary*config=iniparser_load(networkinipath());
	if(!config||!iniparser_find_entry(config,"network"))
	{
		Log(LOGL_WARNING,LOGT_NETWORK, "Network configuration missing from %s, using defaults.\n",networkinipath());
	}
	// TODO: address config
	const struct sockaddr_in server_address=
	{
		.sin_family=AF_INET,
		.sin_port = htons(iniparser_getint(config,"network:port",4444)),
		.sin_addr.s_addr = inet_addr(iniparser_getstring(config,"network:address","127.0.0.1")),
		// padding
		.sin_zero = {0},
	};
	
	//specify kind of socket required.
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// did not get file descriptor?
	if (!server_sockfd)
	{
	    Log(LOGL_ERROR, LOGT_NETWORK, "Network unavailable.\n");
	    return EXIT_FAILURE;
	}

	if(bind(server_sockfd, (struct sockaddr*)&server_address, (socklen_t)sizeof(server_address)))
	{
		Log(LOGL_ERROR,LOGT_NETWORK,"Network Address in use.\n");
	    return EXIT_FAILURE;
	}

	// we now accept connections
	// TODO: expected clients config
	if(listen(server_sockfd, 5))
	{
		Log(LOGL_ERROR,LOGT_NETWORK,"Cannot accept any incoming connections.\n");
		return EXIT_FAILURE;
	}

	/* why are we telling, and who are we telling, their child changed state? 
	if(signal(SIGCHLD, SIG_IGN) == SIG_ERR)
	{
		Log(LOGL_BUG,LOGT_NETWORK,"sigchld wat\n");
		return EXIT_FAILURE;
	}
	do we need this?
	*/

	{
		int*server_ret = malloc(sizeof*server_ret);
		*server_ret = server_sockfd;
		pthread_create(&netthread,NULL,&socklisten,server_ret);
	}
	#endif
	return EXIT_SUCCESS;
}
