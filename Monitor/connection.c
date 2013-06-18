#include "connection.h"
#include <errno.h>

int socklisten()
{
    #ifdef _WIN32
    Sleep(100000000);
    #else
    int server_sockfd, client_sockfd;
    socklen_t server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;


    server_sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    if (! server_sockfd){
        Log(LOGL_ERROR, "error sockfd" );
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    server_address.sin_port = htons( 4444 );

    server_len = (socklen_t)sizeof( server_address );

        if( bind( server_sockfd, ( struct sockaddr *)&server_address, server_len ) != 0 )
        {
                perror("oops: server-tcp-single");
                return EXIT_FAILURE;
        }

    if( listen( server_sockfd, 5 ) ) return EXIT_FAILURE;

    if( signal( SIGCHLD, SIG_IGN ) == SIG_ERR ) return EXIT_FAILURE;

    while( 1 )
    {
        char ch;
        Log(LOGL_SYSTEM_ACTIVITY,"server wait...\n" );

        client_len = (socklen_t)sizeof( client_address );
        if((client_sockfd = accept( server_sockfd, ( struct sockaddr *)&client_address, &client_len ))<1)
        {
            int x = errno;
             Log(LOGL_ERROR, "client sockfd error : %d \n", x);
        }
        Log(LOGL_CLIENT_ACTIVITY, "Client connected \n" );
        
        char*p=malloc(sizeof*p);
        *p='5';
        for (int i=0; i< 20; i++){
            ssize_t x;
        if((x=write(client_sockfd, p, sizeof(char)))!=sizeof(char))
            Log(LOGL_ERROR, "string cannot send %d: %d",x, errno);
        
         Log(LOGL_CLIENT_ACTIVITY, "send char: %c \n", *p );
        }
    //     
        if( fork() == 0 )
        {
            Log(LOGL_CLIENT_ACTIVITY, "in fork loop" );
            if( read( client_sockfd, &ch, 1 ) ) break; 
            Log(LOGL_DEBUG, "Client send = %c\n", ch );
            //
            printf("received: %c\n", ch);
           

            ch++;

            if( sleep( 5 ) ) break;

            Log(LOGL_DEBUG, "Server send = %c\n", ch );
            //if( write( client_sockfd, &ch, 1 ) ) continue;
            if( write( client_sockfd, &ch, 1 ) ) continue;
            printf("send: %c", ch);
            (void)close( client_sockfd );
            return EXIT_SUCCESS;
        }
        else
            Log(LOGL_DEBUG, "socket closed" );
      //  printf("socket closed");
            (void)close( client_sockfd );

    }
    #endif
    return EXIT_FAILURE;
}
