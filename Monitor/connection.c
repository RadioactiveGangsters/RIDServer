#include "connection.h"

void*socklisten(void*connection)
{
	if(!connection)
	{
		Log(LOGT_NETWORK,LOGL_BUG,"Cannot listen without connection");
		pthread_exit(NULL);
	}
	else
	{
		int server_sockfd = *(int*)connection;
		while(1)
		{
			struct sockaddr client_address;
			socklen_t clien_len = (socklen_t)sizeof(client_address);
			int client_sockfd;
			Client*c;

			if((client_sockfd = accept(server_sockfd, &client_address, &clien_len)) < 1 )
			{
				Log(LOGT_NETWORK,LOGL_BUG,"Unable to accept connection, dying.");
				break;
			}
			Log(LOGT_NETWORK,LOGL_CLIENT_ACTIVITY, "Client connected");       

			c=SpawnClient(client_sockfd);
			if(!c)
			{
				Log(LOGT_NETWORK,LOGL_SERIOUS_ERROR,"Out of memory!");
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
		Log(LOGT_NETWORK,LOGL_WARNING, "Network configuration missing from %s, using defaults.",networkinipath());
		iniparser_freedict(config);
	}
	// continue using defaults
	{
		// TODO: address config
		const struct sockaddr_in server_address=
		{
			.sin_family=AF_INET,
			.sin_port = htons((uint16_t)iniparser_getint(config,"network:port",61014)),
			.sin_addr.s_addr = inet_addr(iniparser_getstring(config,"network:address","127.0.0.1")),
			// padding
			.sin_zero = {0},
		};
		
		//specify kind of socket required.
		server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		// did not get file descriptor?
		if (!server_sockfd)
		{
		    Log(LOGT_NETWORK,LOGL_ERROR, "Network unavailable.");
		    iniparser_freedict(config);
		    return EXIT_FAILURE;
		}

		if(bind(server_sockfd, (struct sockaddr*)&server_address, (socklen_t)sizeof(server_address)))
		{
			Log(LOGT_NETWORK,LOGL_ERROR,"Network Address in use.");
			iniparser_freedict(config);
			return EXIT_FAILURE;
		}

		// we now accept connections
		if(listen(server_sockfd, iniparser_getint(config,"network:clients",10)))
		{
			Log(LOGT_NETWORK,LOGL_ERROR,"Cannot accept any incoming connections.");
			iniparser_freedict(config);
			return EXIT_FAILURE;
		}

		iniparser_freedict(config);
		{
			int*server_ret = malloc(sizeof*server_ret);
			*server_ret = server_sockfd;
		    Log(LOGT_NETWORK, LOGL_SYSTEM_ACTIVITY, "Listening for Client connections on port %d.",ntohs(server_address.sin_port));
			pthread_create(&netthread,NULL,&socklisten,server_ret);
		}
	}
	#endif
	return EXIT_SUCCESS;
}
