#include "connection.h"

int socklisten()
{
    #ifdef _WIN32
    Sleep(100000000);
    #else
    int server_sockfd, client_sockfd;
    socklen_t server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (!server_sockfd)
	{
        Log(LOGL_ERROR, LOGT_NETWORK, "Error sockfd\n");
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(4445);

    server_len = (socklen_t)sizeof(server_address);

	if(bind(server_sockfd, (struct sockaddr *)&server_address, server_len) != 0)
    {
    	perror("oops: server-tcp-single");
        return EXIT_FAILURE;
    }

    if(listen(server_sockfd, 5)) return EXIT_FAILURE;
    if(signal(SIGCHLD, SIG_IGN) == SIG_ERR) return EXIT_FAILURE;
       
    typedef  struct
	{
        char opcode;
        char sensortype;
        int maxsensorcount;
        int *sensorwaardes;
    } pack;

    int h,g;
    g = (int) 49;
    h = (int) 52;
    
    pack p = {0x04,'b',2,2};

    pack*pp = malloc(sizeof*pp);
    memcpy(pp, &p,  sizeof*pp);
     
    while(1)
    {
        char ch;
        Log(LOGL_SYSTEM_ACTIVITY, LOGT_NETWORK, "Configuring complete, ready for Client connections..\n");

        client_len = (socklen_t)sizeof(client_address);
        if((client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len)) < 1)
        {
        	int x = errno;
            Log(LOGL_ERROR, LOGT_NETWORK, "Client sockfd error: %d\n", x);
        }
        Log(LOGL_CLIENT_ACTIVITY, LOGT_NETWORK, "Client connected\n");       
        
/*		char*p = malloc(sizeof*p);
        *p ='5'; 
        char*t = malloc(sizeof*t);
        *t ='t';
*/        
        if(fork() == 0)
        {
            Log(LOGL_DEBUG, LOGT_NETWORK, "In fork loop\n");
            MakeClient(client_sockfd);
            return EXIT_SUCCESS;
        }
        else
            Log(LOGL_DEBUG, LOGT_NETWORK, "Socket closed\n");
            (void)close(client_sockfd);
    }
    #endif
    return EXIT_FAILURE;
}
